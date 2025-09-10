// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/BaseEnemy.h"

#include "Components/HealthComponent.h"
#include "Physics/BodyPhysicalMaterial.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

// Sets default values
ABaseEnemy::ABaseEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABaseEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABaseEnemy::ReceiveHitDamage(const FHitDamageInfo& Info)
{
	if (HitBloodEffect)
	{
		UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitBloodEffect, Info.HitLocation, FRotator(0.f), FVector(1), true);
	}

	if (!HealthComponent)
		return;

	if (UBodyPhysicalMaterial* PhysMat = Cast<UBodyPhysicalMaterial>(Info.PhysMat))
	{
		float TotalDamage = Info.BaseDamage * PhysMat->DamageModifier.DamageThresholdMultiplier;
		HealthComponent->TakeDamage(TotalDamage);
	}
}
