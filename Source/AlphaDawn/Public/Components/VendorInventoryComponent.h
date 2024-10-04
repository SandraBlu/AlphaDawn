// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/InventoryComponent.h"
#include "VendorInventoryComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(Inventory), DisplayName = "Vendor Inventory", meta=(BlueprintSpawnableComponent))
class ALPHADAWN_API UVendorInventoryComponent : public UInventoryComponent
{
	GENERATED_BODY()

protected:

	UVendorInventoryComponent(const FObjectInitializer& ObjectInitializer);

	virtual bool AllowLootItem(class UInventoryComponent* Taker, TSubclassOf <class UItem> ItemClass, const int32 Quantity, FText& ErrorText) const override;
	virtual bool AllowStoreItem(class UInventoryComponent* Storer, TSubclassOf <class UItem> ItemClass, const int32 Quantity, FText& ErrorText) const override;

	/** Remove the item from us, and give it to the taker */
	virtual FItemAddResult PerformLootItem(class UInventoryComponent* Taker, TSubclassOf <class UItem> ItemClass, const int32 Quantity = 1) override;

	/** Remove the item from the storer, and give it to us */
	virtual FItemAddResult PerformStoreItem(class UInventoryComponent* Storer, TSubclassOf <class UItem> ItemClass, const int32 Quantity = 1) override;


	/**
	* Return the price we'll buy the given item for
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Item")
	int32 GetBuyPrice(TSubclassOf<class UItem> Item, int32 Quantity = 1) const;
	virtual int32 GetBuyPrice_Implementation(TSubclassOf<class UItem> Item, int32 Quantity = 1) const;

	/**
	* Return the price we'll sell the given item for
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Item")
	int32 GetSellPrice(TSubclassOf<class UItem> Item, int32 Quantity = 1) const;
	virtual int32 GetSellPrice_Implementation(TSubclassOf<class UItem> Item, int32 Quantity = 1) const;

	//The percentage of the items value we'll buy items for
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	float BuyItemPct;

	//The percentage of the items value we'll sell items for 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	float SellItemPct;
	
};
