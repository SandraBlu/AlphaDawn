// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/DInputConfig.h"

const UInputAction* UDInputConfig::FindAbilityInputForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FDInputAction& Action : AbilityInput)
	{
		if (Action.InputAction && Action.InputTag == InputTag)
		{
			return Action.InputAction;
		}
	}
	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("No AbilityInput for InputTag [%s], on InputConfig [%s]"), *InputTag.ToString(), *GetNameSafe(this));
	}
	return nullptr;
}

