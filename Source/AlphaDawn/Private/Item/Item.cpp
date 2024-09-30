// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Item.h"
#include "Components/InventoryComponent.h"
#include "Net/UnrealNetwork.h"
#include "UObject/UnrealType.h"
#include "Engine/BlueprintGeneratedClass.h"

#define LOCTEXT_NAMESPACE "Item"

UItem::UItem()
{
	DisplayName = LOCTEXT("ItemName", "Item");
	UseActionText = LOCTEXT("UseActionText", "Use");
	Weight = 0.f;
	bStackable = true;
	Quantity = 1;
	MaxStackSize = 2;
	RepKey = 0;
	LastUseTime = -FLT_MAX;
	BaseValue = 10;

	FString NameString = GetName();

	//Add the Default Speaker to the Quest 
	int32 UnderscoreIndex = -1;

	if (NameString.FindChar(TCHAR('_'), UnderscoreIndex))
	{
		//remove item name prefix
		DisplayName = FText::FromString(FName::NameToDisplayString(NameString.RightChop(UnderscoreIndex + 1), false));
	}
	else
	{
		DisplayName = FText::FromString(NameString);
	}

	Stats.Add("Weight");
	Stats.Add("Quantity");
}

void UItem::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UItem, Quantity);
	DOREPLIFETIME_CONDITION_NOTIFY(UItem, bActive, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION(UItem, LastUseTime, COND_OwnerOnly);
}

bool UItem::IsSupportedForNetworking() const
{
	return true;
}

void UItem::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	UObject::PostEditChangeProperty(PropertyChangedEvent);
	FName ChangedPropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	//UPROPERTY clamping doesn't support using a variable to clamp so we do in here instead
	if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(UItem, Quantity))
	{
		Quantity = FMath::Clamp(Quantity, 1, GetMaxStackSize());
	}
	else if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(UItem, bStackable))
	{
		if (!bStackable)
		{
			Quantity = 1;
		}
	}
}

bool UItem::HasAuthority() const
{
	return (OwningInventory && OwningInventory->GetOwnerRole() >= ROLE_Authority);
}

class UWorld* UItem::GetWorld() const
{
	return World;
}

void UItem::OnRep_Quantity(const int32 OldQuantity)
{
	OnItemModified.Broadcast();
}

void UItem::OnRep_bActive(const bool bOldActive)
{
	if (bActive)
	{
		Activated();
	}
	else
	{
		Deactivated();
	}
}

void UItem::SetActive(const bool bNewActive)
{
	if (CanActivate() && bNewActive != bActive)
	{
		bActive = bNewActive;
		OnRep_bActive(!bActive);
		MarkDirtyForReplication();
	}
}

void UItem::SetQuantity(const int32 NewQuantity)
{
	if (NewQuantity != Quantity)
	{
		const int32 OldQuantity = Quantity;
		Quantity = FMath::Clamp(NewQuantity, 0, GetMaxStackSize());
		OnRep_Quantity(OldQuantity);
		MarkDirtyForReplication();
	}
}

void UItem::SetLastUseTime(const float NewLastUseTime)
{
	LastUseTime = NewLastUseTime;
	MarkDirtyForReplication();
}

void UItem::Use()
{
}

void UItem::AddedToInventory(UInventoryComponent* Inventory)
{

}

void UItem::RemovedFromInventory(UInventoryComponent* Inventory)
{
	if (bActive)
	{
		Deactivated();
	}
}

void UItem::MarkDirtyForReplication()
{
	//Mark this object for replication
	++RepKey;

	//Mark the array for replication
	if (OwningInventory)
	{
		++OwningInventory->ReplicatedItemsKey;
	}
}

bool UItem::ShouldShowInInventory_Implementation() const
{
	return true;
}

bool UItem::CanBeRemoved_Implementation() const
{
	return true;
}

void UItem::Activated_Implementation()
{
}

void UItem::Deactivated_Implementation()
{
}

bool UItem::CanActivate_Implementation() const
{
	return bCanActivate;
}

bool UItem::CanUse_Implementation() const
{
	return true;
}

FText UItem::GetParsedDescription()
{
	//Replace variables in dialogue line
	FString LineString = Description.ToString();

	int32 OpenBraceIdx = -1;
	int32 CloseBraceIdx = -1;
	bool bFoundOpenBrace = LineString.FindChar('{', OpenBraceIdx);
	bool bFoundCloseBrace = LineString.FindChar('}', CloseBraceIdx);
	uint32 Iters = 0; // More than 50 wildcard replaces and something has probably gone wrong, so safeguard against that

	while (bFoundOpenBrace && bFoundCloseBrace && OpenBraceIdx < CloseBraceIdx && Iters < 50)
	{
		const FString VariableName = LineString.Mid(OpenBraceIdx + 1, CloseBraceIdx - OpenBraceIdx - 1);
		const FString VariableVal = GetStringVariable(VariableName);

		if (!VariableVal.IsEmpty())
		{
			LineString.RemoveAt(OpenBraceIdx, CloseBraceIdx - OpenBraceIdx + 1);
			LineString.InsertAt(OpenBraceIdx, VariableVal);
		}

		bFoundOpenBrace = LineString.FindChar('{', OpenBraceIdx);
		bFoundCloseBrace = LineString.FindChar('}', CloseBraceIdx);

		Iters++;
	}

	return FText::FromString(LineString);
}

FString UItem::GetStringVariable_Implementation(const FString& VariableName)
{
	//Overriable in BP in case you want to add more 
	if (VariableName == "Display Name")
	{
		return DisplayName.ToString();
	}
	else if (VariableName == "Weight")
	{
		return FString::SanitizeFloat(Weight);
	}
	else if (VariableName == "Recharge Duration")
	{
		return FString::SanitizeFloat(UseRechargeDuration);
	}
	else if (VariableName == "Stack Weight")
	{
		return FString::SanitizeFloat(GetStackWeight());
	}
	else if (VariableName == "Quantity")
	{
		return FString::FromInt(Quantity);
	}
	else if (VariableName == "Max Stack Size")
	{
		return FString::FromInt(MaxStackSize);
	}
	else if (VariableName == "BaseValue")
	{
		return FString::FromInt(BaseValue);
	}

	return FString();
}

APlayerController* UItem::GetOwningController() const
{
	if (OwningInventory)
	{
		return OwningInventory->GetOwningController();
	}

	return nullptr;
}

APawn* UItem::GetOwningPawn() const
{
	if (OwningInventory)
	{
		return OwningInventory->GetOwningPawn();
	}
	return nullptr;
}

#undef LOCTEXT_NAMESPACE
