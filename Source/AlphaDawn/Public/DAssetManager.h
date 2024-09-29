// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "DAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class ALPHADAWN_API UDAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:

	static UDAssetManager& Get();

protected:

	virtual void StartInitialLoading() override;
	
};
