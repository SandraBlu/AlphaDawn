// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BFLInventory.generated.h"

/**
 * 
 */
UCLASS()
class ALPHADAWN_API UBFLInventory : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory", meta = (DefaultToSelf = "Target"))
	static class UInventoryComponent* GetInventoryComponentFromTarget(AActor* Target);

	
	//Sort the array of inventory items from a-z
	UFUNCTION(BlueprintPure, Category = "Inventory")
	static TArray<class UItem*> SortItemArrayAlphabetical(TArray<class UItem*> InItems, const bool bReverse);


	//Sort the array of inventory items using stack weight
	UFUNCTION(BlueprintPure, Category = "Inventory")
	static TArray<class UItem*> SortItemArrayWeight(TArray<class UItem*> InItems, const bool bReverse);
	
};
