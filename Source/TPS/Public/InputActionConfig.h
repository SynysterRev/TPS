// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "InputActionConfig.generated.h"

class UInputAction;


USTRUCT(BlueprintType)
struct FInputActionConfig
{
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, Category = "Input")
    TObjectPtr<const UInputAction> Action = nullptr;

    UPROPERTY(EditAnywhere, Category = "Input")
    FName ActionName;

    UPROPERTY(EditAnywhere, Category = "Input")
    TMap<FName, ETriggerEvent> EventAndFunctionName;
};
