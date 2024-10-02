// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/DAbilitySystemGlobals.h"

#include "AbilityEffectTypes.h"

FGameplayEffectContext* UDAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FDGameplayEffectContext();
}
