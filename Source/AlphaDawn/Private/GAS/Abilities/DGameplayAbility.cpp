// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/DGameplayAbility.h"

#include "GAS/DAttributeSet.h"

FString UDGameplayAbility::GetDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>%s, </><Level>%d</>"),
	L"Default Ability Name - LoremIpsum ", Level);
}

FString UDGameplayAbility::GetNextLevelDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>Next Level: </><Level>%d</> \n<Default>Causes much more damage. </>"), Level);
}

FString UDGameplayAbility::GetLockedDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>Ability Locked Until Level: %d</>"), Level);
}

float UDGameplayAbility::GetEnergyCost(float InLevel) const
{
	float EnergyCost = 0.f;
	if (const UGameplayEffect* CostEffect = GetCostGameplayEffect())
	{
		for (FGameplayModifierInfo Mod : CostEffect->Modifiers)
		{
			if (Mod.Attribute == UDAttributeSet::GetEnergyAttribute())
			{
				Mod.ModifierMagnitude.GetStaticMagnitudeIfPossible(InLevel, EnergyCost);
				break;
			}
		}
	}
	return EnergyCost;
}

float UDGameplayAbility::GetCooldown(float InLevel) const
{
	float Cooldown = 0.f;
	if (const UGameplayEffect* CooldownEffect = GetCooldownGameplayEffect())
	{
		CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(InLevel, Cooldown);
	}
	return Cooldown;
}
