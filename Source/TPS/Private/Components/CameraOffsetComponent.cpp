// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CameraOffsetComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Curves/CurveFloat.h"

#include "TPSCharacter.h"
#include "Weapons/TPSBaseWeapon.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UCameraOffsetComponent::UCameraOffsetComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UCameraOffsetComponent::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0))
	{
		CameraManager->ViewPitchMin = -55.0f;
		CameraManager->ViewPitchMax = 75.0f;
	}

	if (ATPSCharacter* Character = Cast<ATPSCharacter>(GetOwner()))
	{
		OwnerCharacter = Character;
	}
}

void UCameraOffsetComponent::UpdateCameraOffset(float DeltaTime) const
{
	if (!CameraBoom || !OwnerCharacter) return;

	// Check this to modify it later, I don't like it
	FRotator ControlRot = OwnerCharacter->GetControlRotation();
	float Pitch = FMath::UnwindDegrees(ControlRot.Pitch);
	// Only when lookup to the top
	//if (Pitch > PitchStartOffset) // Avoid micro adjusment
	//{
	//	// Calculate offset
	//	float UpwardLookRatio = FMath::Clamp((Pitch - 10.0f) / 50.0f, 0.0f, 1.0f);

	//	// Apply offset to socket
	//	FVector NewOffset = BaseCameraOffset + UpwardOffset * UpwardLookRatio;
	//	CameraBoom->SocketOffset = FMath::VInterpTo(CameraBoom->SocketOffset, NewOffset, DeltaTime, InterpSpeed);
	//	CameraBoom->TargetOffset = FMath::VInterpTo(CameraBoom->TargetOffset, TargetOffset * UpwardLookRatio, DeltaTime,
	//	                                            InterpSpeed);
	//}
	//else
	//{
	//	// Back to normal
	//	CameraBoom->SocketOffset = FMath::VInterpTo(CameraBoom->SocketOffset, BaseCameraOffset, DeltaTime, InterpSpeed);
	//}
}

void UCameraOffsetComponent::UpdateAimSway(float DeltaTime, bool bInIsAiming)
{
	if (!CameraBoom || !OwnerCharacter) return;

	if (bInIsAiming)
	{
		// Change this to get it only once when the player equip a new weapon
		ATPSBaseWeapon* CurrentWeapon = OwnerCharacter->GetCurrentWeapon();

		if (!CurrentWeapon) return;

		FSwayProperties SwayProperties = CurrentWeapon->GetSwayProperties();

		float Time = GetWorld()->GetTimeSeconds() * SwayProperties.SwayCurveSpeed;
	
		float PitchOffset = SwayProperties.SwayCurvePitch->GetFloatValue(FMath::Fmod(Time, 2.0f));
		float YawOffset = SwayProperties.SwayCurveYaw->GetFloatValue(FMath::Fmod(Time, 1.2f));
	
		FRotator BaseRot = OwnerCharacter->GetControlRotation();
	
		FRotator SwayRot;
		SwayRot.Pitch = PitchOffset * SwayProperties.SwayAmount;
		SwayRot.Yaw = YawOffset * SwayProperties.SwayAmount;
	
		FRotator FinalRot = BaseRot + SwayRot;
		FollowCamera->SetRelativeRotation(SwayRot);
	}
}

void UCameraOffsetComponent::HandleAiming(float DeltaTime, bool bInIsAiming)
{
	if (!FollowCamera || !ZoomedViewTarget || !BaseCameraTarget)
		return;

	if (bInIsAiming && !bIsTransitioning)
	{
		TargetLocation = ZoomedViewTarget->GetComponentLocation();
		TargetRotation = ZoomedViewTarget->GetComponentRotation();

		TargetFOV = DefaultFOV - FOVZoomReduction;
	}
	else
	{
		TargetLocation = BaseCameraTarget->GetComponentLocation();
		TargetRotation = BaseCameraTarget->GetComponentRotation();
		TargetFOV = DefaultFOV;
	}

	FollowCamera->SetWorldLocation(FMath::VInterpTo(FollowCamera->GetComponentLocation(), TargetLocation, DeltaTime,
	                                                AimSpeed));
	FollowCamera->SetWorldRotation(FMath::RInterpTo(FollowCamera->GetComponentRotation(), TargetRotation, DeltaTime,
	                                                AimSpeed));

	FollowCamera->SetFieldOfView(FMath::FInterpTo(FollowCamera->FieldOfView, TargetFOV, DeltaTime, FOVZoomSpeed));
	if (IsTransitionComplete())
	{
		// event
		bIsTransitioning = false;
	}
}

bool UCameraOffsetComponent::IsTransitionComplete() const
{
	return FVector::Dist(FollowCamera->GetComponentLocation(), TargetLocation) < TransitionZoomTolerance && FMath::Abs(
		FollowCamera->GetComponentRotation().GetManhattanDistance(TargetRotation)) < TransitionZoomTolerance;
}

void UCameraOffsetComponent::InitializeCameraView(const FCameraViewConfig& Config)
{
	FollowCamera = Config.FollowCamera;
	CameraBoom = Config.CameraBoom;
	BaseCameraTarget = Config.BaseCameraTarget;
	ZoomedViewTarget = Config.ZoomedViewTarget;
}
