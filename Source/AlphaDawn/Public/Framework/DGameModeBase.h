// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DGameModeBase.generated.h"

class UAbilityInfo;
class UCharacterClassInfo;
/**
 * 
 */
UCLASS()
class ALPHADAWN_API ADGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	UCharacterClassInfo* CharacterClassInfo;

	UPROPERTY(EditDefaultsOnly, Category = "Ability Info")
	UAbilityInfo* AbilityInfo;
	
};
