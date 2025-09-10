// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class TPS_API UMainHUDWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> CurrentAmmoInMag;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TotalAmmo;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UCanvasPanel> CrossHairCanvas;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> TopCrossHair;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> RightCrossHair;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> BottomCrossHair;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> LeftCrossHair;

	UFUNCTION()
	void UpdateAmmoDisplay(int32 AmmoInMag, int32 InTotalAmmo);

	UFUNCTION()
	virtual void EnableCrosshair(bool bVisible);

	UFUNCTION()
	void UpdateCrosshair(float Velocity);

	void ResetCrosshair();
	
	void UpdateMaxAmmo();

public:

	virtual void InitializeWeaponUI(class ATPSCharacter* InCharacter);
};
