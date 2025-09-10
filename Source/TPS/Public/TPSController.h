// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TPSController.generated.h"

/**
 * 
 */
UCLASS()
class TPS_API ATPSController : public APlayerController
{
	GENERATED_BODY()

private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY()
	class UMainHUDWidget* WeaponUIWidget = nullptr;

protected:

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(BlueprintReadOnly, Category = Character)
	TObjectPtr<class ATPSCharacter> PlayerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI)
	TSubclassOf<class UMainHUDWidget> WeaponUIClass;

public:

	void InitializeWeaponUI();
	
};
