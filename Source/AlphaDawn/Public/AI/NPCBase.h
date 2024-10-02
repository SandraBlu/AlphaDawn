// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/DCharacterBase.h"
#include "UI/GAS/Controllers/OverlayWidgetController.h"
#include "NPCBase.generated.h"

class UBehaviorTree;
class ADAIController;
class UPawnSensingComponent;
class UWidgetComponent;
/**
 * 
 */
UCLASS()
class ALPHADAWN_API ANPCBase : public ADCharacterBase
{
	GENERATED_BODY()

public:

	ANPCBase();

	virtual void PossessedBy(AController* NewController) override;

	//Combat Interface
	virtual int32 GetPlayerLevel_Implementation() override;
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& CombatSocketTag) override;
	virtual void Die(const FVector& DeathImpulse) override;
	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;
	virtual AActor* GetCombatTarget_Implementation() const override;

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath(const FVector& DeathImpulse);

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangeSignature OnHealthChange;
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangeSignature OnMaxHealthChange;

	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	
	UFUNCTION(BlueprintCallable)
	void SetToStunned(bool bIsStunned) const;

	UFUNCTION(BlueprintCallable)
	void SetToEndStun();
	
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting = false;

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	bool bStunned = false;

	UFUNCTION(BlueprintImplementableEvent)
	void DissolveMesh();

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	float BaseWalkSpeed = 250.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float LifeSpan = 5.f;

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	AActor* CombatTarget;
	
protected:
	
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeAttributes() const override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UWidgetComponent* HealthBar;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	USkeletalMeshComponent* Weapon;

	//Weapon Damage Socket
	UPROPERTY(EditAnywhere, Category = "Combat")
	FName WeaponDamageSocket;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;

	UPROPERTY(EditAnywhere, Category = "AI")
	UBehaviorTree* BehaviorTree;

	UPROPERTY()
	ADAIController* AIC;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;
	
};