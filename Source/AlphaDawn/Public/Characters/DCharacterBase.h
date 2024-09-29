// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "DCharacterBase.generated.h"

class UGameplayAbility;
class UGameplayEffect;
class UAttributeSet;

UCLASS()
class ALPHADAWN_API ADCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	
	ADCharacterBase();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }


protected:
	
	virtual void BeginPlay() override;
	UPROPERTY()
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	UAttributeSet* AttributeSet;

	virtual void InitAbilityActorInfo();
	//void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;
	virtual void InitializeAttributes() const;
	void GrantAbilities();
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="GAS|Attributes")
	TSubclassOf<UGameplayEffect>PrimaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="GAS|Attributes")
	TSubclassOf<UGameplayEffect>SecondaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="GAS|Attributes")
	TSubclassOf<UGameplayEffect>ResistanceAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="GAS|Attributes")
	TSubclassOf<UGameplayEffect>VitalAttributes;
	

private:
	
	UPROPERTY(EditAnywhere, Category="GAS|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> GrantedAbilities;

	UPROPERTY(EditAnywhere, Category="GAS|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> GrantedPassiveAbilities;

	UPROPERTY(EditAnywhere, Category="Combat")
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditAnywhere, Category="Combat")
	UAnimMontage* StunnedMontage;
	
	
	

};
