// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/ProjectileAbility.h"

#include "DGameplayTags.h"
#include "Actors/Projectiles/OverlapMissile.h"
#include "Interfaces/CombatInterface.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

UProjectileAbility::UProjectileAbility()
{
	SweepRadius = 20.0f;
	SweepDistanceFallback = 5000;
}

void UProjectileAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	const bool bIsServer = HasAuthority(&ActivationInfo);
	if (!bIsServer) return;
}

void UProjectileAbility::SpawnProjectile(ACharacter* InstigatorCharacter, const FGameplayTag& SocketTag)
{
	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), SocketTag);
	// Ignore Player
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(InstigatorCharacter);
	FCollisionShape Shape;
	Shape.SetSphere(SweepRadius);
	FVector TraceDirection = InstigatorCharacter->GetControlRotation().Vector();

	// We trace against the environment first to find whats under the player crosshair.
	// We use the hit location to adjust the projectile launch direction so it will hit what is under the crosshair rather than shoot straight forward from the socket
	// Add sweep radius onto start to avoid the sphere clipping into floor/walls the camera is directly against.
	FVector TraceStart = InstigatorCharacter->GetPawnViewLocation() + (TraceDirection * SweepRadius);
	// endpoint far into the look-at distance (not too far, still adjust somewhat towards crosshair on a miss)
	FVector TraceEnd = TraceStart + (TraceDirection * SweepDistanceFallback);

	FHitResult Hit;
	// returns true if we got to a blocking hit (Channel1="Projectile" defined in DefaultGame.ini)
	if (GetWorld()->SweepSingleByChannel(Hit, TraceStart, TraceEnd, FQuat::Identity, ECC_GameTraceChannel1, Shape, Params))
	{
		// Overwrite trace end with impact point in world
		TraceEnd = Hit.ImpactPoint;
	}
	// find new direction/rotation from Hand pointing to impact point in world.
	FRotator ProjRotation = (TraceEnd - SocketLocation).Rotation();
	FTransform SpawnTransform = FTransform(ProjRotation, SocketLocation);
	SpawnTransform.SetLocation(SocketLocation);

	AOverlapMissile* Missile = GetWorld()->SpawnActorDeferred<AOverlapMissile>(ProjectileClass, SpawnTransform, GetOwningActorFromActorInfo(), Cast<APawn>(GetOwningActorFromActorInfo()),
	ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	
	Missile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
	Missile->FinishSpawning(SpawnTransform);
}
