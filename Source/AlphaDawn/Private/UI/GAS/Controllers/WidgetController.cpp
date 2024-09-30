// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GAS/Controllers/WidgetController.h"

#include "Framework/DPlayerController.h"
#include "Framework/DPlayerState.h"
#include "GAS/DAbilitySystemComponent.h"
#include "GAS/DAttributeSet.h"
#include "GAS/DataAssets/AbilityInfo.h"

void UWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void UWidgetController::BroadcastInitialValues()
{
}

void UWidgetController::BindCallbacksToDependencies()
{
}

void UWidgetController::BroadcastAbilityInfo()
{
	if (!GetDASC()->bGrantedAbilitiesGiven) return;
	
	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this](const FGameplayAbilitySpec& AbilitySpec)
	{
		FAbilityInformation Info = AbilityInfo->FindAbilityInfoForTag(DASC->GetAbilityTagFromSpec(AbilitySpec));
		Info.InputTag = DASC->GetInputTagFromSpec(AbilitySpec);
		Info.StatusTag = DASC->GetStatusFromSpec(AbilitySpec);
		AbilityInfoDelegate.Broadcast(Info);
	});
	GetDASC()->ForEachAbility(BroadcastDelegate);
}

ADPlayerController* UWidgetController::GetDPC()
{
	if (DPC == nullptr)
	{
		DPC = Cast<ADPlayerController>(PlayerController);
	}
	return DPC;
}

ADPlayerState* UWidgetController::GetDPS()
{
	if (DPS == nullptr)
	{
		DPS = Cast<ADPlayerState>(PlayerState);
	}
	return DPS;
}

UDAbilitySystemComponent* UWidgetController::GetDASC()
{
	if (DASC == nullptr)
	{
		DASC = Cast<UDAbilitySystemComponent>(AbilitySystemComponent);
	}
	return DASC;
}

UDAttributeSet* UWidgetController::GetDAS()
{
	if (DAS == nullptr)
	{
		DAS = Cast<UDAttributeSet>(AttributeSet);
	}
	return DAS;
}
