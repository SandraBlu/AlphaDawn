// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DPlayerController.generated.h"

class UDamageTextWidget;
/**
 * 
 */
UCLASS()
class ALPHADAWN_API ADPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	
	ADPlayerController();
	
	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bDodgedHit, bool bCriticalHit);
	
private:
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextWidget> DamageTextWidgetClass;
};
