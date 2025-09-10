// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/TPSBaseWeapon.h"
#include "TPSPistol.generated.h"

/**
 * 
 */
UCLASS()
class TPS_API ATPSPistol : public ATPSBaseWeapon
{
	GENERATED_BODY()

public:
	virtual void Fire() override;
	virtual void Reload() override;
	
};
