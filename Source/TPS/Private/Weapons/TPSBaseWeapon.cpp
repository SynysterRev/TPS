// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/TPSBaseWeapon.h"

#include "TPSCharacter.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
ATPSBaseWeapon::ATPSBaseWeapon()
{
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	Mesh->SetupAttachment(RootComponent);

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATPSBaseWeapon::BeginPlay()
{
	Super::BeginPlay();
}

bool ATPSBaseWeapon::CanFire()
{
	return !bIsReloading && CurrentAmmoInMagazine > 0 && FireTimer <= 0.0f;
}

bool ATPSBaseWeapon::HasAmmoInReserve()
{
	return CurrentAmmoReserve > 0;
}

// Called every frame
void ATPSBaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (FireTimer > 0.0f)
	{
		FireTimer -= DeltaTime;
	}
	if (bIsFiring && CanFire())
	{
		Fire();
	}
}

void ATPSBaseWeapon::TryFire()
{
	if (CanFire())
	{
		bIsFiring = true;
		Fire();
	}
	else if (CurrentAmmoInMagazine == 0)
	{
		Reload();
	}
}

void ATPSBaseWeapon::Fire()
{
	FireTimer = FiringTime;
	if (OwnerCharacter)
	{
		OwnerCharacter->PlayAnimMontage(FireMontage);
	}
}

void ATPSBaseWeapon::ReleaseFire()
{
	bIsFiring = false;
}

void ATPSBaseWeapon::Reload()
{
	if (HasAmmoInReserve())
	{
		bIsReloading = true;
		FireTimer = 0.0f;
		if (OwnerCharacter)
		{
			OwnerCharacter->PlayAnimMontage(ReloadMontage);
		}
	}
}

void ATPSBaseWeapon::InitWeapon(class ATPSCharacter* InCharacter)
{
	OwnerCharacter = InCharacter;
}

int32 ATPSBaseWeapon::GetAmmoInMag() const
{
	return CurrentAmmoInMagazine;
}

int32 ATPSBaseWeapon::GetTotalAmmoReserve() const
{
	return TotalAmmoReserve;
}

void ATPSBaseWeapon::OnEndReload()
{
	int32 AmmoNeeded = MagazineCapacity - CurrentAmmoInMagazine;
	int32 AmmoToLoad = FMath::Min(AmmoNeeded, CurrentAmmoReserve);
	CurrentAmmoInMagazine += AmmoToLoad;
	CurrentAmmoReserve -= AmmoToLoad;
	bIsReloading = false;

	OnAmmoChanged.Broadcast(CurrentAmmoInMagazine, CurrentAmmoReserve);
}
