// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GAS/DUserWidget.h"

void UDUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
