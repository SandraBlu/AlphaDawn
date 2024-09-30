// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GAS/Controllers/OverlayWidgetController.h"

#include "DGameplayTags.h"
#include "Framework/DPlayerState.h"
#include "GAS/DAbilitySystemComponent.h"
#include "GAS/DAttributeSet.h"
#include "GAS/DataAssets/AbilityInfo.h"
#include "GAS/DataAssets/LevelUpInfo.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	OnHealthChange.Broadcast(GetDAS()->GetHealth());
	OnMaxHealthChange.Broadcast(GetDAS()->GetMaxHealth());
	OnStaminaChange.Broadcast(GetDAS()->GetStamina());
	OnMaxStaminaChange.Broadcast(GetDAS()->GetMaxStamina());
	OnEnergyChange.Broadcast(GetDAS()->GetEnergy());
	OnMaxEnergyChange.Broadcast(GetDAS()->GetMaxEnergy());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	//XP Change
	GetDPS()->OnXPChangeDelegate.AddUObject(this, &UOverlayWidgetController::OnXPChange);
	GetDPS()->OnLevelChangeDelegate.AddLambda([this](int32 NewLevel)
	{
		OnLevelChangeDelegate.Broadcast(NewLevel);
	});
	//Attribute Change
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetDAS()->GetHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data){OnHealthChange.Broadcast(Data.NewValue);});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetDAS()->GetMaxHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data){OnMaxHealthChange.Broadcast(Data.NewValue);});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetDAS()->GetStaminaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data){OnStaminaChange.Broadcast(Data.NewValue);});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetDAS()->GetMaxStaminaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data){OnMaxStaminaChange.Broadcast(Data.NewValue);});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetDAS()->GetEnergyAttribute()).AddLambda([this](const FOnAttributeChangeData& Data){OnEnergyChange.Broadcast(Data.NewValue);});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetDAS()->GetMaxEnergyAttribute()).AddLambda([this](const FOnAttributeChangeData& Data){OnMaxEnergyChange.Broadcast(Data.NewValue);});

	 //Ability Change
	if (GetDASC())
	{
		GetDASC()->AbilityEquippedDelegate.AddUObject(this, &UOverlayWidgetController::OnAbilityEquipped);
		if (GetDASC()->bGrantedAbilitiesGiven)
		{
			BroadcastAbilityInfo();
		}
		else
		{
			GetDASC()->AbilityGivenDelegate.AddUObject(this, &UOverlayWidgetController::BroadcastAbilityInfo);
		}

		//Effect Message
    	GetDASC()->EffectAssetTags.AddLambda(
    		[this](const FGameplayTagContainer& AssetTags)
    		{
    			for (const FGameplayTag& Tag : AssetTags)
    			{
    				FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("message"));
    				if (Tag.MatchesTag(MessageTag))
    				{
    					const FUIMessageRow* Row = GetDataTableRowByTag<FUIMessageRow>(MessageWidgetDataTable, Tag);
    					MessageWidgetDelegate.Broadcast(*Row);
    				}
    			}
    		}
    	);	
	}
}

void UOverlayWidgetController::OnXPChange(int32 NewXP)
{
	const ULevelUpInfo* LevelUpInfo = GetDPS()->LevelUpInfo;
	checkf(LevelUpInfo, TEXT("Unable to find XPinfo, Enter in BP_PlayerState"));

	const int32 CurrentLevel = LevelUpInfo->FindLevelForXP(NewXP);
	const int32 MaxLevel = LevelUpInfo->LevelUpInfo.Num();

	if (CurrentLevel <= MaxLevel && CurrentLevel >0)
	{
		const int32 LevelUpReq = LevelUpInfo->LevelUpInfo[CurrentLevel].LevelUpRequirement;
		const int32 PrevLevelUpReq = LevelUpInfo->LevelUpInfo[CurrentLevel -1].LevelUpRequirement;

		const int32 DeltaLevelReq = LevelUpReq - PrevLevelUpReq;
		const int32 XPForThisLevel = NewXP - PrevLevelUpReq;

		const float XPBarPercent = static_cast<float>(XPForThisLevel)/ static_cast<float>(DeltaLevelReq);

		OnXPPercentChangeDelegate.Broadcast(XPBarPercent);
	}
}

void UOverlayWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status,
	const FGameplayTag& Slot, const FGameplayTag& PrevSlot) const
{
	//Clear Out Old Slot
	const FDGameplayTags Tag = FDGameplayTags::Get();
	FAbilityInformation PrevSlotInfo;
	PrevSlotInfo.StatusTag = Tag.ability_status_unlocked;
	PrevSlotInfo.InputTag = PrevSlot;
	PrevSlotInfo.AbilityTag =Tag.ability_none;
	AbilityInfoDelegate.Broadcast(PrevSlotInfo);
	
	//Fill new slot
	FAbilityInformation Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	Info.StatusTag = Status;
	Info.InputTag = Slot;
	AbilityInfoDelegate.Broadcast(Info);
}
