// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/DPlayerController.h"

#include "GameFramework/Character.h"
#include "UI/GAS/DamageTextWidget.h"

void ADPlayerController::ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bDodgedHit, bool bCriticalHit)
{
	if (IsValid(TargetCharacter) && DamageTextWidgetClass && IsLocalController())
	{
		UDamageTextWidget* DamageText = NewObject<UDamageTextWidget>(TargetCharacter, DamageTextWidgetClass);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageText->SetDamageText(DamageAmount, bBlockedHit, bDodgedHit, bCriticalHit);
	}
}

ADPlayerController::ADPlayerController()
{
	bReplicates = true;
}
