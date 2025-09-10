// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TPSBaseWeapon.generated.h"

class USkeletalMeshComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAmmoChanged, int32, AmmoInMag, int32, AmmoReserve);

USTRUCT(BlueprintType)
struct FSwayProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Aim Sway", BlueprintReadWrite)
	UCurveFloat* SwayCurvePitch = nullptr;

	UPROPERTY(EditAnywhere, Category = "Aim Sway", BlueprintReadWrite)
	UCurveFloat* SwayCurveYaw = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SwayAmount = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SwayCurveSpeed = 1.0f;
};

UCLASS()
class TPS_API ATPSBaseWeapon : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(Category = Weapon, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Mesh;

public:
	// Sets default values for this actor's properties
	ATPSBaseWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = Weapon)
	float Damage = 1.0f;

	UPROPERTY(EditAnywhere, Category = Weapon)
	float FiringTime = 0.7f;

	UPROPERTY(EditAnywhere, Category = Weapon)
	float ReloadTime = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TotalAmmoReserve = 90;

	UPROPERTY(EditAnywhere, Category = Weapon)
	int32 MagazineCapacity = 10;

	UPROPERTY(EditAnywhere, Category = Weapon)
	float MaxAmmoDistance = 8000.0f;

	int32 CurrentAmmoReserve = 90;
	int32 CurrentAmmoInMagazine = 10;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAnimMontage> FireMontage;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAnimMontage> ReloadMontage;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAnimMontage> WeaponFireMontage;

	UPROPERTY(BlueprintReadWrite)
	bool bIsReloading = false;
	
	bool bIsFiring = false;
	float FireTimer = 0.0f;
	virtual bool CanFire();
	virtual void Fire();

	UPROPERTY(BlueprintReadOnly)
	class ATPSCharacter* OwnerCharacter = nullptr;

	virtual bool HasAmmoInReserve();

	UFUNCTION(BlueprintImplementableEvent)
	void OnProjectHitEvent(FVector HitLocation, FVector ImpactNormal);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSwayProperties SwayProperties;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void TryFire();
	virtual void ReleaseFire();
	virtual void Reload();

	virtual void InitWeapon(class ATPSCharacter* InCharacter);

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnAmmoChanged OnAmmoChanged;

	int32 GetAmmoInMag() const;
	int32 GetTotalAmmoReserve() const;

	UFUNCTION(BlueprintCallable)
	virtual void OnEndReload();

	FSwayProperties& GetSwayProperties() { return SwayProperties; }

	FORCEINLINE USkeletalMeshComponent* GetMesh() const { return Mesh; }
};
