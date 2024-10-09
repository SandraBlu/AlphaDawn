// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/DamageAbility.h"
#include "MissileAbility.generated.h"

class AOverlapMissile;
/**
 * 
 */
UCLASS()
class ALPHADAWN_API UMissileAbility : public UDamageAbility
{
	GENERATED_BODY()

public:

	UMissileAbility();

	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;

protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditAnywhere, Category="Targeting")
	float SweepRadius;

	/* Fallback distance when sweep finds no collision under crosshair. Adjusts final projectile direction */
	UPROPERTY(EditAnywhere, Category="Targeting")
	float SweepDistanceFallback;
	
	UPROPERTY(EditDefaultsOnly)
	int32 NumProjectiles = 5;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AOverlapMissile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly)
	int32 MaxNumProjectiles = 5;
	
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float HomingAccelerationMin = 1600.f;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float HomingAccelerationMax = 3200.f;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	bool bLaunchHomingProjectiles = true;

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SpawnProjectiles(ACharacter* InstigatorCharacter, bool bOverridePitch, float PitchOverride, AActor* HomingTarget);

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	static FHitResult GetHit(bool bBlockingHit, class AActor* HitActor);
	
};
