// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GAS/AlphaHUD.h"

#include "UI/GAS/DUserWidget.h"
#include "UI/GAS/Controllers/AbilityMenuController.h"
#include "UI/GAS/Controllers/AttributeMenuController.h"
#include "UI/GAS/Controllers/OverlayWidgetController.h"

UOverlayWidgetController* AAlphaHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController ==nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbacksToDependencies();
	}
	return OverlayWidgetController;
}

UAttributeMenuController* AAlphaHUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (AttributeMenuController == nullptr)
	{
		AttributeMenuController = NewObject<UAttributeMenuController>(this, AttributeMenuControllerClass);
		AttributeMenuController->SetWidgetControllerParams(WCParams);
		AttributeMenuController->BindCallbacksToDependencies();
	}
	return AttributeMenuController;
}

UAbilityMenuController* AAlphaHUD::GetAbilityMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (AbilityMenuController == nullptr)
	{
		AbilityMenuController = NewObject<UAbilityMenuController>(this, AbilityMenuControllerClass);
		AbilityMenuController->SetWidgetControllerParams(WCParams);
		AbilityMenuController->BindCallbacksToDependencies();
	}
	return AbilityMenuController;
}

void AAlphaHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized, please fill out BP_HUD"));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class uninitialized, please fill out BP_HUD"));

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UDUserWidget>(Widget);

	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	OverlayWidget->SetWidgetController(WidgetController);
	WidgetController->BroadcastInitialValues();
	Widget->AddToViewport();
}
