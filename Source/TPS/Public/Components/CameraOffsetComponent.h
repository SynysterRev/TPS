// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CameraOffsetComponent.generated.h"

struct FCameraViewConfig;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class TPS_API UCameraOffsetComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCameraOffsetComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	bool IsTransitionComplete() const;

	UPROPERTY(BlueprintReadWrite)
	class USpringArmComponent* CameraBoom;

	UPROPERTY(BlueprintReadWrite)
	class UCameraComponent* FollowCamera;

	UPROPERTY(BlueprintReadWrite)
	USceneComponent* BaseCameraTarget = nullptr;

	UPROPERTY(BlueprintReadWrite)
	USceneComponent* ZoomedViewTarget = nullptr;

	UPROPERTY(BlueprintReadWrite)
	class ATPSCharacter* OwnerCharacter;

	UPROPERTY(EditAnywhere)
	FVector BaseCameraOffset;

	UPROPERTY(EditAnywhere)
	FVector UpwardOffset = FVector(0.0f, 50.0f, 0.0f);

	UPROPERTY(EditAnywhere)
	FVector TargetOffset = FVector(0.0f, 10.0f, 0.0f);

	UPROPERTY(EditAnywhere)
	float PitchStartOffset;

	UPROPERTY(EditAnywhere)
	float InterpSpeed = 5.0f;

	UPROPERTY(EditAnywhere, Category = Aiming)
	float AimSpeed = 5.0f;

	UPROPERTY(EditAnywhere, Category = Aiming)
	float TransitionZoomTolerance = 0.1f;

	UPROPERTY(EditAnywhere, Category = Aiming)
	float FOVZoomReduction = 20.0f;

	float DefaultFOV = 70.0f;

	UPROPERTY(EditAnywhere, Category = Aiming)
	float FOVZoomSpeed = 3.0f;

	bool bIsTransitioning;
	FVector TargetLocation;
	FRotator TargetRotation;
	float TargetFOV;

public:
	
	void InitializeCameraView(const FCameraViewConfig& Config);
	void UpdateCameraOffset(float DeltaTime) const;
	void UpdateAimSway(float DeltaTime, bool bInIsAiming);
	void HandleAiming(float DeltaTime, bool bInIsAiming);
};
