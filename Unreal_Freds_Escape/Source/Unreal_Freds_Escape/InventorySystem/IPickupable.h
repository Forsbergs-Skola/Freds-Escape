// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InventoryItemData.h"
#include "IPickupable.generated.h"

/**
 * Implement this on any world actor that can be stored in the inventory
 * alongside with IInteractable. IInteractable handles the pick-up and inspect of the item
 * IPickupable provides the item data for the inventory entry
 *
 * In Blueprint: override GetItemData and return the UInventoryItemData
 * asset you created for this prop. That is the only thing the BP needs to do
 */

UINTERFACE(MinimalAPI, Blueprintable)
class UPickupable : public UInterface
{
	GENERATED_BODY()
};


class UNREAL_FREDS_ESCAPE_API IPickupable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Interaction|Inventory")
	UInventoryItemData* GetItemData();
	virtual UInventoryItemData* GetItemData_Implementation() { return nullptr; }

	UFUNCTION(BlueprintNativeEvent, Category = "Interaction|Inventory")
	bool OnPickup(APlayerController* Player);
	virtual bool OnPickup_Implementation(APlayerController* Player) { return true;  }

	UFUNCTION(BlueprintNativeEvent, Category = "Interaction|Inventory")
	void OnDrop(APlayerController* Player);
	virtual void OnDrop_Implementation(APlayerController* Player) {}
};