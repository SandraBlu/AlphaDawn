// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/FireBlast.h"

#include "Actors/Projectiles/Fireball.h"
#include "Framework/BFLAbilitySystem.h"

FString UFireBlast::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float EnergyCost = FMath::Abs(GetEnergyCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
			// Title
			"<Title>FIRE BLAST</>\n\n"

			// Level
			"<Small>Level: </><Level>%d</>\n"
			// ManaCost
			"<Small>EnergyCost: </><EnergyCost>%.1f</>\n"
			// Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			// Number of Fire Balls
			"<Default>Launches %d </>"
			"<Default>fire balls in all directions, each coming back and </>"
			"<Default>exploding upon return, causing </>"

			// Damage
			"<Damage>%d</><Default> radial fire damage with"
			" a chance to burn</>"),

			// Values
			Level,
			EnergyCost,
			Cooldown,
			NumFireballs,
			ScaledDamage);
}

FString UFireBlast::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float EnergyCost = FMath::Abs(GetEnergyCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
			// Title
			"<Title>NEXT LEVEL:</>\n\n"

			// Level
			"<Small>Level: </><Level>%d</>\n"
			// ManaCost
			"<Small>EnergyCost: </><EnergyCost>%.1f</>\n"
			// Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			// Number of Fire Balls
			"<Default>Launches %d </>"
			"<Default>fire balls in all directions, each coming back and </>"
			"<Default>exploding upon return, causing </>"

			// Damage
			"<Damage>%d</><Default> radial fire damage with"
			" a chance to burn</>"),

			// Values
			Level,
			EnergyCost,
			Cooldown,
			NumFireballs,
			ScaledDamage);
}

TArray<AFireball*> UFireBlast::SpawnFireballs()
{
	TArray<AFireball*> Fireballs;
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location =GetAvatarActorFromActorInfo()->GetActorLocation();
	TArray<FRotator> Rotators = UBFLAbilitySystem::EvenlySpacedRotators(Forward, FVector::UpVector, 360.f, NumFireballs);

	for (const FRotator& Rotator : Rotators)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(Location);
		SpawnTransform.SetRotation(Rotator.Quaternion());
		
		AFireball* Fireball = GetWorld()->SpawnActorDeferred<AFireball>(FireballClass, SpawnTransform, GetOwningActorFromActorInfo(),
			CurrentActorInfo->PlayerController->GetPawn(), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		Fireball->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		Fireball->ReturnToActor = GetAvatarActorFromActorInfo();
		Fireball->SetOwner(GetAvatarActorFromActorInfo());
		
		Fireball->ExplosionDamageParams = MakeDamageEffectParamsFromClassDefaults();
		Fireball->SetOwner(GetAvatarActorFromActorInfo());
		
		Fireballs.Add(Fireball);
		Fireball->FinishSpawning(SpawnTransform);
	}
	return Fireballs;
}