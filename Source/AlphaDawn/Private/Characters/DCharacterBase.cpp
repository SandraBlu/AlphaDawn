// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/DCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"

// Sets default values
ADCharacterBase::ADCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

UAbilitySystemComponent* ADCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

// Called when the game starts or when spawned
void ADCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADCharacterBase::InitAbilityActorInfo()
{
}

// void ADCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
// {
// 	check(IsValid(GetAbilitySystemComponent()));
// 	check(GameplayEffectClass);
// 	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
// 	ContextHandle.AddSourceObject(this);
// 	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
// 	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
// }

void ADCharacterBase::InitializeAttributes() const
{
	//ApplyEffectToSelf(PrimaryAttributes, 1.f);
}

void ADCharacterBase::GrantAbilities()
{
	//URAbilitySystemComponent* RASC = CastChecked<URAbilitySystemComponent>(AbilitySystemComponent);
	//if (!HasAuthority()) return;
	//RASC->AddGrantedAbilities(GrantedAbilities);
	//RASC->AddPassiveAbilities(GrantedPassiveAbilities);
}


