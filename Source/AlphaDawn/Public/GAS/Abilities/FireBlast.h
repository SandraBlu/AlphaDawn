// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/DamageAbility.h"
#include "FireBlast.generated.h"

class AFireball;
/**
 * 
 */
UCLASS()
class ALPHADAWN_API UFireBlast : public UDamageAbility
{

	GENERATED_BODY()

public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;
	
	UFUNCTION(BlueprintCallable)
	TArray<AFireball*> SpawnFireballs();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "FireBlast")
	int32 NumFireballs = 12;

private:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AFireball> FireballClass;
	
};
