// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/MissileAbility.h"

#include "DGameplayTags.h"
#include "Actors/Projectiles/OverlapMissile.h"
#include "Framework/BFLAbilitySystem.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Character.h"
#include "Interfaces/CombatInterface.h"

UMissileAbility::UMissileAbility()
{
	SweepRadius = 20.0f;
	SweepDistanceFallback = 5000;
}

FString UMissileAbility::GetDescription(int32 Level)
{
	return FString();
}

FString UMissileAbility::GetNextLevelDescription(int32 Level)
{
	return FString();
}

void UMissileAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	const bool bIsServer = HasAuthority(&ActivationInfo);
	if (!bIsServer) return;
}

void UMissileAbility::SpawnProjectiles(ACharacter* InstigatorCharacter, bool bOverridePitch, float PitchOverride, AActor* HomingTarget)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;
	
	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), FDGameplayTags::Get().combatSocket_weapon);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(InstigatorCharacter);
	FCollisionShape Shape;
	Shape.SetSphere(SweepRadius);
	
	FVector TraceDirection = InstigatorCharacter->GetControlRotation().Vector();
	FVector TraceStart = InstigatorCharacter->GetPawnViewLocation() + (TraceDirection * SweepRadius);
	FVector TraceEnd = TraceStart + (TraceDirection * SweepDistanceFallback);
	FHitResult Hit;
	if (GetWorld()->SweepSingleByChannel(Hit, TraceStart, TraceEnd, FQuat::Identity, ECC_GameTraceChannel1, Shape, Params))
	{
		TraceEnd = Hit.Location;
	}
	FRotator Rotation = (TraceEnd - SocketLocation).Rotation();
	if (bOverridePitch) Rotation.Pitch = PitchOverride;
	
	const FVector Forward = Rotation.Vector();
	const int32 EffectiveNumMissiles = FMath::Min(NumProjectiles, GetAbilityLevel());
	
	TArray<FRotator> Rotations = UBFLAbilitySystem::EvenlySpacedRotators(Forward, FVector::UpVector, SweepRadius, EffectiveNumMissiles);

	for (const FRotator& Rot : Rotations)
	{
		FTransform SpawnTransform = FTransform(Rot, SocketLocation);
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rot.Quaternion());
		
		AOverlapMissile* Missile = GetWorld()->SpawnActorDeferred<AOverlapMissile>(ProjectileClass, SpawnTransform, GetOwningActorFromActorInfo(), Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	
		Missile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		if (HomingTarget && HomingTarget->Implements<UCombatInterface>())
		{
			Missile->MovementComp->HomingTargetComponent = HomingTarget->GetRootComponent();
		}
		else
		{
			Missile->HomingTarget = NewObject<USceneComponent>(USceneComponent::StaticClass());
			Missile->HomingTarget->SetWorldLocation(TraceEnd);
			Missile->MovementComp->HomingTargetComponent = Missile->HomingTarget;
		}
		Missile->MovementComp->HomingAccelerationMagnitude = FMath::FRandRange(HomingAccelerationMin, HomingAccelerationMax);
		Missile->MovementComp->bIsHomingProjectile = bLaunchHomingProjectiles;
		Missile->FinishSpawning(SpawnTransform);
	}
}

FHitResult UMissileAbility::GetHit(bool bBlockingHit, class AActor* HitActor)
{
	FHitResult Hit;
	Hit.bBlockingHit = bBlockingHit;
	Hit.HitObjectHandle = FActorInstanceHandle(HitActor);
	return Hit;
}
