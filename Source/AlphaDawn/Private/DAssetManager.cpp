// Fill out your copyright notice in the Description page of Project Settings.


#include "DAssetManager.h"

#include "DGameplayTags.h"

UDAssetManager& UDAssetManager::Get()
{
	check(GEngine)
	UDAssetManager* RAssetManager = Cast<UDAssetManager>(GEngine->AssetManager);
	return *RAssetManager;
}

void UDAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FDGameplayTags::InitializeNativeGameplayTags();
}
