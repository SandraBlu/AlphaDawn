// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Projectiles/Fireball.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Framework/BFLAbilitySystem.h"

void AFireball::BeginPlay()
{
	Super::BeginPlay();
	StartOutgoingTimeline();
}

void AFireball::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValidOverlap(OtherActor)) return;

	if (HasAuthority())
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			const FVector DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
			DamageEffectParams.DeathImpulse = DeathImpulse;
			const bool bKnockback = FMath::RandRange(1, 100) <DamageEffectParams.KnockbackChance;
			
			DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
			UBFLAbilitySystem::ApplyDamageEffect(DamageEffectParams);
		}
	}
}

void AFireball::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}
