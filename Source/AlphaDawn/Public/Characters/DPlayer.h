// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/DCharacterBase.h"
#include "DPlayer.generated.h"

class UInputMappingContext;
class UDAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class ALPHADAWN_API ADPlayer : public ADCharacterBase
{
	GENERATED_BODY()
	
public:
    	
    	ADPlayer();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

protected:
	
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UCameraComponent* FollowCam;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* PlayerMappingContext;

private:
	virtual void InitAbilityActorInfo() override;

	UPROPERTY()
	UDAbilitySystemComponent* DAbilitySystemComponent;

	UDAbilitySystemComponent* GetASC();

public:
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
};
