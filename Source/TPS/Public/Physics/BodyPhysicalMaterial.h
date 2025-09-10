// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "BodyPhysicalMaterial.generated.h"

UENUM(BlueprintType)
enum class EHitZone : uint8
{
	None UMETA(DisplayName = "None"),
	Head UMETA(DisplayName = "Head"),
	Torso UMETA(DisplayName = "Torso"),
	Arm UMETA(DisplayName = "Arm"),
	Leg UMETA(DisplayName = "Leg")
};

UCLASS(BlueprintType)
class TPS_API UBodyPhysicalMaterial : public UPhysicalMaterial
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Detection")
	EHitZone HitZone = EHitZone::None;
};
