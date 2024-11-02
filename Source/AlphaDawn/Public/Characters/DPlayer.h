// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/DCharacterBase.h"
#include "GameplayTagContainer.h"
#include "Interfaces/PlayerInterface.h"
#include "DPlayer.generated.h"

class AWeapon;
class UEquipmentComponent;
class UInputMappingContext;
class UDAbilitySystemComponent;
class UDInputConfig;
/**
 * 
 */
UCLASS()
class ALPHADAWN_API ADPlayer : public ADCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()
	
public:
    	
	ADPlayer();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UEquipmentComponent* Gear;
	
	UPROPERTY()
	AWeapon* Weapon;

	//Player Interface
	virtual int32 GetPlayerLevel_Implementation() override;
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& CombatSocketTag) override;
	virtual void Die(const FVector& DeathImpulse) override;
	virtual void AddToXP_Implementation(int32 InXP) override;
	virtual void LevelUp_Implementation() override;
	virtual int32 GetXP_Implementation() const override;
	virtual int32 FindLevelForXP_Implementation(int32 InXP) const override;
	virtual int32 GetAttributePtsReward_Implementation(int32 Level) const override;
	virtual int32 GetAbilityPtsReward_Implementation(int32 Level) const override;
	virtual void AddToPlayerLevel_Implementation(int32 InPlayerLevel) override;
	virtual void AddToAttributePts_Implementation(int32 InAttributePoints) override;
	virtual void AddToAbilityPts_Implementation(int32 InAbilityPoints) override;
	virtual int32 GetAttributePoints_Implementation() const override;
	virtual int32 GetAbilityPoints_Implementation() const override;
	virtual AWeapon* GetCurrentWeapon_Implementation() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UNiagaraComponent* LevelUpFX;
	
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath(const FVector& DeathImpulse);

protected:
	
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* PlayerMappingContext;
	
	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	AActor* CombatTarget;

private:
	virtual void InitAbilityActorInfo() override;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UDInputConfig* InputConfig;

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UPROPERTY()
	UDAbilitySystemComponent* DAbilitySystemComponent;

	UDAbilitySystemComponent* GetASC();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastLevelUpVFX();

public:
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
};
