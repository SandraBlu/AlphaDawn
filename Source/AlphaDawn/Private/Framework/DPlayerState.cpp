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
	//PlayerInventory->SetCapacity(25);
	//PlayerInventory->SetWeightCapacity(60.f);

	NetUpdateFrequency = 100.f;
}

void ADPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

UAbilitySystemComponent* ADPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
