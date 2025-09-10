// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "InputActionConfig.h"
#include "TPSCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UCameraOffsetComponent;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnChangeWeapon, int32, AmmoInMag, int32, AmmoReserve);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAim, bool, bIsAiming);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMoveWhenAiming, float, Velocity);

USTRUCT(BlueprintType)
struct FCameraViewConfig
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	UCameraComponent* FollowCamera = nullptr;

	UPROPERTY(BlueprintReadWrite)
	USpringArmComponent* CameraBoom = nullptr;

	UPROPERTY(BlueprintReadWrite)
	USceneComponent* BaseCameraTarget = nullptr;

	UPROPERTY(BlueprintReadWrite)
	USceneComponent* ZoomedViewTarget = nullptr;
};

UCLASS(config=Game)
class ATPSCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USceneComponent* BaseCameraTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USceneComponent* ZoomedViewTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraOffsetComponent* CameraOffsetComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TArray<FInputActionConfig> InputActions;

public:
	ATPSCharacter();

	UPROPERTY(BlueprintReadOnly, Category = Weapon)
	bool bIsArmed = false;

	UPROPERTY(BlueprintReadOnly, Category = Weapon)
	bool bIsAiming = false;

	UPROPERTY()
	FOnChangeWeapon OnChangeWeapon;

	UPROPERTY()
	FOnAim OnAim;

	UPROPERTY()
	FOnMoveWhenAiming OnMoveWhenAiming;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void Move(const FInputActionValue& Value);

	UFUNCTION()
	void Look(const FInputActionValue& Value);

	UFUNCTION()
	void Fire() const;

	UFUNCTION()
	void ReleaseFire();

	UFUNCTION()
	void Reload() const;

	UFUNCTION()
	void Aim();

	UFUNCTION()
	void StopAim();

protected:
	//virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void BindInputActions(class UEnhancedInputComponent* EnhancedInputComponent);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Weapon)
	TSubclassOf<class ATPSBaseWeapon> Weapon;

	UPROPERTY(BlueprintReadOnly, Category = Weapon)
	class ATPSBaseWeapon* CurrentWeapon = nullptr;

	UPROPERTY(EditAnywhere, Category = Weapon)
	float TurnSpeed = 5.0f;

private:

	void HandleAiming(float DeltaTime);
	bool bIsTransitioning = false;
	
public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FORCEINLINE class ATPSBaseWeapon* GetCurrentWeapon() const { return CurrentWeapon; }
};
