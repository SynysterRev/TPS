// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainHUDWidget.h"

#include "TPSCharacter.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Weapons/TPSBaseWeapon.h"

void UMainHUDWidget::UpdateAmmoDisplay(int32 AmmoInMag, int32 InTotalAmmo)
{
	if (CurrentAmmoInMag)
	{
		CurrentAmmoInMag->SetText(FText::FromString(FString::FromInt(AmmoInMag)));
	}
	if (TotalAmmo)
	{
		TotalAmmo->SetText(FText::FromString(FString::FromInt(InTotalAmmo)));
	}
}

void UMainHUDWidget::UpdateMaxAmmo()
{
}

void UMainHUDWidget::InitializeWeaponUI(ATPSCharacter* InCharacter)
{
	if (!InCharacter)
		return;


	ATPSBaseWeapon* Weapon = InCharacter->GetCurrentWeapon();

	if (!Weapon)
		return;

	Weapon->OnAmmoChanged.AddDynamic(this, &UMainHUDWidget::UpdateAmmoDisplay);
	InCharacter->OnChangeWeapon.AddDynamic(this, &UMainHUDWidget::UpdateAmmoDisplay);
	InCharacter->OnAim.AddDynamic(this, &UMainHUDWidget::EnableCrosshair);
	InCharacter->OnMoveWhenAiming.AddDynamic(this, &UMainHUDWidget::UpdateCrosshair);
	UpdateAmmoDisplay(Weapon->GetAmmoInMag(), Weapon->GetTotalAmmoReserve());

	if (CrossHairCanvas)
	{
		CrossHairCanvas->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UMainHUDWidget::EnableCrosshair(bool bVisible)
{
	if (CrossHairCanvas)
	{
		CrossHairCanvas->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		if (!bVisible)
		{
			ResetCrosshair();
		}
	}
}

void UMainHUDWidget::UpdateCrosshair(float Velocity)
{
	float CrossHairSpread =
		FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 450.0f), FVector2D(0.0f, 60.0f), Velocity);
	if (TopCrossHair)
	{
		if (UCanvasPanelSlot* TopSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(TopCrossHair))
			TopSlot->SetPosition(FVector2D(0.0f, -CrossHairSpread));
	}
	if (RightCrossHair)
	{
		if (UCanvasPanelSlot* RightSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(RightCrossHair))
			RightSlot->SetPosition(FVector2D(CrossHairSpread, 0.0f));
	}
	if (BottomCrossHair)
	{
		if (UCanvasPanelSlot* BottomSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(BottomCrossHair))
			BottomSlot->SetPosition(FVector2D(0.0f, CrossHairSpread));
	}
	if (LeftCrossHair)
	{
		if (UCanvasPanelSlot* LeftSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(LeftCrossHair))
			LeftSlot->SetPosition(FVector2D(-CrossHairSpread, 0.0f));
	}
}

void UMainHUDWidget::ResetCrosshair()
{
	if (TopCrossHair)
	{
		if (UCanvasPanelSlot* TopSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(TopCrossHair))
			TopSlot->SetPosition(FVector2D(0.0f, 0.0f));
	}
	if (RightCrossHair)
	{
		if (UCanvasPanelSlot* RightSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(RightCrossHair))
			RightSlot->SetPosition(FVector2D(0.0f, 0.0f));
	}
	if (BottomCrossHair)
	{
		if (UCanvasPanelSlot* BottomSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(BottomCrossHair))
			BottomSlot->SetPosition(FVector2D(0.0f, 0.0f));
	}
	if (LeftCrossHair)
	{
		if (UCanvasPanelSlot* LeftSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(LeftCrossHair))
			LeftSlot->SetPosition(FVector2D(-0.0f, 0.0f));
	}
}
