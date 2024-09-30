// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/DPlayerState.h"

#include <GAS/DAbilitySystemComponent.h>
#include "Components/InventoryComponent.h"
#include "GAS/DAttributeSet.h"
#include "Net/UnrealNetwork.h"

ADPlayerState::ADPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UDAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UDAttributeSet>("AttributeSet");

	PlayerInventory = CreateDefaultSubobject<UInventoryComponent>("InventoryComp");
	PlayerInventory->SetCapacity(25);
	PlayerInventory->SetWeightCapacity(60.f);

	NetUpdateFrequency = 100.f;
}

void ADPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADPlayerState, Level);
	DOREPLIFETIME(ADPlayerState, XP);
	DOREPLIFETIME(ADPlayerState, AttributePoints);
	DOREPLIFETIME(ADPlayerState, AbilityPoints);
}

UAbilitySystemComponent* ADPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ADPlayerState::AddToXP(int32 InXP)
{
	XP += InXP;
	OnXPChangeDelegate.Broadcast(XP);
}

void ADPlayerState::AddToLevel(int32 InLevel)
{
	Level += InLevel;
	OnLevelChangeDelegate.Broadcast(Level);
}

void ADPlayerState::AddToAttributePts(int32 InAttributePts)
{
	AttributePoints += InAttributePts;
	OnAttributePtsChangeDelegate.Broadcast(AttributePoints);
}

void ADPlayerState::AddToAbilityPts(int32 InAbilityPts)
{
	AbilityPoints += InAbilityPts;
	OnAbilityPtsChangeDelegate.Broadcast(AbilityPoints);
}

void ADPlayerState::SetXP(int32 InXP)
{
	XP = InXP;
	OnXPChangeDelegate.Broadcast(XP);
}

void ADPlayerState::SetLevel(int32 InLevel)
{
	Level = InLevel;
	OnLevelChangeDelegate.Broadcast(Level);
}

void ADPlayerState::OnRep_Level(int32 OldLevel)
{
	OnLevelChangeDelegate.Broadcast(Level);
}

void ADPlayerState::OnRep_XP(int32 OldXP)
{
	OnXPChangeDelegate.Broadcast(XP);
}

void ADPlayerState::OnRep_AttributePoints(int32 OldAttributePoints)
{
	OnAttributePtsChangeDelegate.Broadcast(AttributePoints);
}

void ADPlayerState::OnRep_AbilityPoints(int32 OldAbilityPoints)
{
	OnAbilityPtsChangeDelegate.Broadcast(AbilityPoints);
}
