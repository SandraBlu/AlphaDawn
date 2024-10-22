// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "NiagaraSystem.h"
#include "GameFramework/Character.h"
#include "GAS/DataAssets/CharacterClassInfo.h"
#include "Interfaces/CombatInterface.h"
#include "DCharacterBase.generated.h"

class UDebuffNiagaraComponent;
class UGameplayAbility;
class UGameplayEffect;
class UAttributeSet;

UCLASS()
class ALPHADAWN_API ADCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	
	ADCharacterBase();
	virtual void Tick(float DeltaSeconds) override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	//Combat Interface
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual UAnimMontage* GetStunnedMontage_Implementation() override;
	virtual bool IsDead_Implementation() const override;
	virtual AActor* GetAvatar_Implementation() override;
	virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() override;
	virtual UNiagaraSystem* GetBloodEffect_Implementation() override;
	virtual ECharacterClass GetCharacterClass_Implementation() override;
	virtual FOnASCRegistered GetOnASCRegisteredDelegate() override;
	virtual FOnDeath GetOnDeathDelegate() override;
	virtual void Die(const FVector& DeathImpulse) override;
	virtual FOnDamageSignature& GetOnDamageSignature() override;
	//Combat Interface
	
	FOnASCRegistered OnASCRegistered;
	FOnDeath OnDeath;
	FOnDamageSignature OnDamageDelegate;
	
protected:
	
	virtual void BeginPlay() override;
	
	UPROPERTY()
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	UAttributeSet* AttributeSet;

	virtual void InitAbilityActorInfo();
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Ranger;
	
	bool bDead = false;
	
	UPROPERTY(EditAnywhere, Category= "Combat")
	TArray<FTaggedMontage> AttackMontages;
	
	UPROPERTY(EditAnywhere, Category="Combat")
	FName HandRSocket;
    
	UPROPERTY(EditAnywhere, Category="Combat")
	FName HandLSocket;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	UNiagaraSystem* BloodEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	USoundBase* DeathCry;

	UPROPERTY(VisibleAnywhere)
	UDebuffNiagaraComponent* EffectDebuffComponent;

private:
	
	UPROPERTY(EditAnywhere, Category="GAS|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> GrantedAbilities;

	UPROPERTY(EditAnywhere, Category="GAS|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> GrantedPassiveAbilities;

	UPROPERTY(EditAnywhere, Category="Combat")
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditAnywhere, Category="Combat")
	UAnimMontage* StunnedMontage;

	//Passive Effects Niagara
	//UPROPERTY(VisibleAnywhere, Category="PassiveEffects")
	//UPassiveNiagaraComponent* PassiveCloak;
	
	UPROPERTY(VisibleAnywhere, Category="PassiveEffects")
	USceneComponent* EffectAttachComp;

};
