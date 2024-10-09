// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/DamageAbility.h"
#include "FireGrenade.generated.h"

/**
 * 
 */
UCLASS()
class ALPHADAWN_API UFireGrenade : public UDamageAbility
{
	GENERATED_BODY()
public:
	
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;
	
};
