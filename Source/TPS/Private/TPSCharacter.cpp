// Copyright Epic Games, Inc. All Rights Reserved.

#include "TPSCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CameraOffsetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "TPSController.h"
#include "Kismet/GameplayStatics.h"
#include "Weapons/TPSBaseWeapon.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ATPSCharacter

ATPSCharacter::ATPSCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 220.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	// Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	BaseCameraTarget = CreateDefaultSubobject<USceneComponent>(TEXT("BaseCameraTarget"));
	BaseCameraTarget->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	ZoomedViewTarget = CreateDefaultSubobject<USceneComponent>(TEXT("ZoomedViewTarget"));
	ZoomedViewTarget->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	ZoomedViewTarget->SetRelativeLocation(FVector(80.0f, 0.0f, 0.0f));

	CameraOffsetComp = CreateDefaultSubobject<UCameraOffsetComponent>(TEXT("CameraOffsetComponent"));

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

//void ATPSCharacter::NotifyControllerChanged()
//{
//	Super::NotifyControllerChanged();
//
//	// Add Input Mapping Context
//	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
//	{
//		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
//		{
//			Subsystem->AddMappingContext(DefaultMappingContext, 0);
//		}
//	}
//}

void ATPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (UWorld* world = GetWorld())
	{
		FTransform Tr = GetMesh()->GetSocketTransform("WeaponsSocket");
		FVector Location = Tr.GetLocation();
		FQuat Rotation = Tr.GetRotation();
		FActorSpawnParameters SpawnInfo;
		CurrentWeapon = world->SpawnActor<ATPSBaseWeapon>(Weapon, Location, Rotation.Rotator(), SpawnInfo);
		FAttachmentTransformRules AttachementRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget,
		                                           EAttachmentRule::KeepWorld, true);
		CurrentWeapon->AttachToComponent(GetMesh(), AttachementRules, "RightHandSocket");
		CurrentWeapon->InitWeapon(this);
		bIsArmed = true;
		OnChangeWeapon.Broadcast(CurrentWeapon->GetAmmoInMag(), CurrentWeapon->GetTotalAmmoReserve());

		Cast<ATPSController>(GetController())->InitializeWeaponUI();
	}
	if (CameraOffsetComp)
	{
		FCameraViewConfig CamConfig;
		CamConfig.BaseCameraTarget = BaseCameraTarget;
		CamConfig.ZoomedViewTarget = ZoomedViewTarget;
		CamConfig.FollowCamera = FollowCamera;
		CamConfig.CameraBoom = CameraBoom;
		CameraOffsetComp->InitializeCameraView(CamConfig);
	}
}

void ATPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HandleAiming(DeltaTime);
	if (CameraOffsetComp)
	{
		CameraOffsetComp->UpdateCameraOffset(DeltaTime);
		CameraOffsetComp->HandleAiming(DeltaTime, bIsAiming);
		CameraOffsetComp->UpdateAimSway(DeltaTime, bIsAiming);
	}
}

void ATPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		BindInputActions(EnhancedInputComponent);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error,
		       TEXT(
			       "'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."
		       ), *GetNameSafe(this));
	}
}

void ATPSCharacter::BindInputActions(UEnhancedInputComponent* EnhancedInputComponent)
{
	for (const FInputActionConfig& InputConfig : InputActions)
	{
		if (!InputConfig.Action) continue;

		for (const auto& TriggerFunc : InputConfig.EventAndFunctionName)
		{
			if (TriggerFunc.Key.IsNone())
				continue;
			EnhancedInputComponent->BindAction(InputConfig.Action, TriggerFunc.Value, this, TriggerFunc.Key);
		}
	}
}

void ATPSCharacter::HandleAiming(float DeltaTime)
{
	if (bIsAiming)
	{
		FRotator ControlRot = GetControlRotation();
		ControlRot.Pitch = 0.f;
		SetActorRotation(FMath::RInterpTo(GetActorRotation(), ControlRot, DeltaTime, TurnSpeed));
		
		OnMoveWhenAiming.Broadcast(GetVelocity().Length());
	}
}

void ATPSCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ATPSCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>() * 0.1f;

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ATPSCharacter::Fire() const
{
	if (CurrentWeapon)
	{
		CurrentWeapon->TryFire();
	}
}

void ATPSCharacter::ReleaseFire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->ReleaseFire();
	}
}

void ATPSCharacter::Reload() const
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Reload();
	}
}

void ATPSCharacter::Aim()
{
	OnAim.Broadcast(true);
	bIsTransitioning = true;
	bIsAiming = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
}

void ATPSCharacter::StopAim()
{
	OnAim.Broadcast(false);
	bIsTransitioning = true;
	bIsAiming = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}
