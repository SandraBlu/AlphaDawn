// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/DGameplayAbility.h"
#include "PassiveAbility.generated.h"

/**
 * 
 */
UCLASS()
class ALPHADAWN_API UPassiveAbility : public UDGameplayAbility
{
	GENERATED_BODY()

public:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	void ReceiveDeactivate(const FGameplayTag& AbilityTag);
	
};
