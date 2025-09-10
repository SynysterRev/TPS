// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSController.h"
#include "TPSCharacter.h"
#include "UI/MainHUDWidget.h"
#include "EnhancedInputSubsystems.h"

void ATPSController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	PlayerCharacter = Cast<ATPSCharacter>(GetCharacter());

	if (WeaponUIClass && GetWorld())
	{
		WeaponUIWidget = CreateWidget<UMainHUDWidget>(GetWorld(), WeaponUIClass);
		if (WeaponUIWidget)
		{
			WeaponUIWidget->AddToViewport();
		}
	}
}

void ATPSController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	//if (PlayerCharacter)
	//{
	//    if (UCharacterStatusComponent* StatusComponent = PlayerCharacter->GetStatusComponent())
	//    {
	//        StatusComponent->LifeChangeEvent.Remove(this, FName("OnHealthChanged"));
	//        StatusComponent->StaminaChangeEvent.Remove(this, FName("OnStaminaChanged"));
	//    }
	//}
	Super::EndPlay(EndPlayReason);
}

void ATPSController::InitializeWeaponUI()
{
	if (WeaponUIWidget && PlayerCharacter)
	{
		WeaponUIWidget->InitializeWeaponUI(PlayerCharacter);
	}
}
