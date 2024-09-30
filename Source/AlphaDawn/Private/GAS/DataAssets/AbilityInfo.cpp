// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/DataAssets/AbilityInfo.h"

FAbilityInformation UAbilityInfo::FindAbilityInfoForTag(const FGameplayTag& AbilityTag, bool bLogNotFound) const
{
	for (const FAbilityInformation& Info : AbilityInfo)
	{
		if (Info.AbilityTag == AbilityTag)
		{
			return Info;
		}
	}
	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't find info for AbilityTag[%s] on AbilityInfo [%s]"), *AbilityTag.ToString(), *GetNameSafe(this));
	}
	return FAbilityInformation();
}
