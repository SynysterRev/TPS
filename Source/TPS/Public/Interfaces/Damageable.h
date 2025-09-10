// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Damageable.generated.h"

USTRUCT(BlueprintType)
struct FHitDamageInfo
{
	GENERATED_BODY()

	UPROPERTY()
	float BaseDamage = 0.f;

	UPROPERTY()
	FName HitBone;

	UPROPERTY()
	UPhysicalMaterial* PhysMat = nullptr;

	UPROPERTY()
	FVector HitLocation = FVector(0);

	UPROPERTY()
	AActor* InstigatorActor = nullptr;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDamageable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TPS_API IDamageable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void ReceiveHitDamage(const FHitDamageInfo& Info) = 0;
};
