// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/PassiveAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GAS/DAbilitySystemComponent.h"

void UPassiveAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                      const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (UDAbilitySystemComponent* DASC = Cast<UDAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo())))
	{
		DASC->DeactivatePassiveAbilityDelegate.AddUObject(this, &UPassiveAbility::ReceiveDeactivate);
	}
}

void UPassiveAbility::ReceiveDeactivate(const FGameplayTag& AbilityTag)
{
	if (AbilityTags.HasTagExact(AbilityTag))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}
