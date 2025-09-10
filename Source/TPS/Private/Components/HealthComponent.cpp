// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HealthComponent.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
}

void UHealthComponent::TakeDamage(float Amount)
{
	if (IsDead()) return;

	const float OldHealth = CurrentHealth;
	CurrentHealth = FMath::Clamp(CurrentHealth - Amount, 0.f, MaxHealth);
	OnHealthChanged.Broadcast(CurrentHealth, CurrentHealth - OldHealth);
	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Example text that prints a float: %f"), CurrentHealth));
	if (CurrentHealth <= 0.f)
	{
		OnDeath.Broadcast(GetOwner());
		// to change and make clearer
		ACharacter* Character = Cast<ACharacter>(GetOwner());
		Character->GetMesh()->SetSimulatePhysics(true);
		Character->GetCapsuleComponent()->DestroyComponent();
	}
}

void UHealthComponent::Heal(float Amount)
{
	if (IsDead()) return;

	const float OldHealth = CurrentHealth;
	CurrentHealth = FMath::Clamp(CurrentHealth + Amount, 0.f, MaxHealth);
	OnHealthChanged.Broadcast(CurrentHealth, CurrentHealth - OldHealth);
}

bool UHealthComponent::IsDead() const
{
	return CurrentHealth <= 0.f;
}

