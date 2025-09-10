// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapons/TPSPistol.h"
#include "TPSCharacter.h"
#include "Camera/CameraComponent.h"
#include "Interfaces/Damageable.h"
#include "TPS/TPS.h"

void ATPSPistol::Fire()
{
	UWorld* World = GetWorld();
	if (!World)
		return;

	if (CanFire())
	{
		Super::Fire();
		if (OwnerCharacter)
		{
			FVector Forward = OwnerCharacter->GetFollowCamera()->GetForwardVector();
			FVector Location = OwnerCharacter->GetFollowCamera()->GetComponentLocation();
			FVector EndLocation = Location + Forward * MaxAmmoDistance;
			FCollisionQueryParams Params;
			Params.bReturnPhysicalMaterial = true;
			FHitResult Hit;
			if (World->LineTraceSingleByChannel(Hit, Location, EndLocation, CollisionChannels::Weapon, Params))
			{
				if (AActor* HitActor = Hit.GetActor())
				{
					FHitDamageInfo Info;
					Info.BaseDamage = Damage;
					Info.HitBone = Hit.BoneName;
					Info.PhysMat = Hit.PhysMaterial.Get();
					Info.HitLocation = Hit.ImpactPoint;
					Info.InstigatorActor = GetOwner();

					if (HitActor->Implements<UDamageable>())
					{
						Cast<IDamageable>(HitActor)->ReceiveHitDamage(Info);
					}
				}
				OnProjectHitEvent(Hit.Location, Hit.ImpactNormal);
				// DrawDebugLine(GetWorld(), Location, Hit.Location, FColor::Green, false, 5, 0, 1);
				DrawDebugSphere(GetWorld(), Hit.Location, 2.f, 8, FColor::Green, false, -1, 0, 1);
			}
		}
		CurrentAmmoInMagazine--;
		OnAmmoChanged.Broadcast(CurrentAmmoInMagazine, CurrentAmmoReserve);
	}
}

void ATPSPistol::Reload()
{
	Super::Reload();
}
