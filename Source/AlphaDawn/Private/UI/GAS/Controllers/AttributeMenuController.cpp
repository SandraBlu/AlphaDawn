// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GAS/Controllers/AttributeMenuController.h"

#include "DGameplayTags.h"
#include "Framework/DPlayerState.h"
#include "GAS/DAbilitySystemComponent.h"
#include "GAS/DAttributeSet.h"
#include "GAS/DataAssets/AttributeInfo.h"

void UAttributeMenuController::BroadcastInitialValues()
{
	check(AttributeInfo);
	for (auto& Pair : GetDAS()->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}
	AttributePointsChangeDelegate.Broadcast(GetDPS()->GetAttributePts());
}

void UAttributeMenuController::BindCallbacksToDependencies()
{
	check(AttributeInfo);
	for (auto& Pair : GetDAS()->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda([this, Pair](const FOnAttributeChangeData& Data)
		{
			BroadcastAttributeInfo(Pair.Key, Pair.Value());
		});	
	}
	GetDPS()->OnAttributePtsChangeDelegate.AddLambda([this] (int32 Points)
	{
		AttributePointsChangeDelegate.Broadcast(Points);
	});
}

void UAttributeMenuController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	GetDASC()->UpgradeAttribute(AttributeTag);
}

void UAttributeMenuController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const
{
	FAttributeData Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
