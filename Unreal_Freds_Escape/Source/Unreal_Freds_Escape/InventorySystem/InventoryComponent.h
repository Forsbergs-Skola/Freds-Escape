// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryItemData.h"
#include "InventoryComponent.generated.h"


// One slot in the inventory
USTRUCT(BlueprintType)
struct FInventorySlot
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	UInventoryItemData* ItemData = nullptr;

	bool IsEmpty() const
	{
		return ItemData == nullptr;
	}
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemUsed, UInventoryItemData*, ItemData);


/**
 * Manages the player's inventory
 * Attach to the Character via code (done in EscapeCharacter)
 * WBP_Inventory binds to OnInventoryChanged and calls GetSlots() to refresh
 */


UCLASS(ClassGroup = (Inventory), meta = (BlueprintSpawnableComponent))
class UNREAL_FREDS_ESCAPE_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryComponent();

	// Max number of slots
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 MaxSlots = 4;

	// Broadcast whenever the inventory changes and bind this in WBP_Inventory
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryChanged OnInventoryChanged;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnItemUsed OnItemUsed;

	// Allow all item types to enter this inventory
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<UInventoryItemData*> AllowedItems;

	// Returns the full slot array for UI binding
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	TArray<FInventorySlot> GetSlots() const { return Slots;  }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	bool HasItem(UInventoryItemData* ItemData) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	bool HasRoom() const;

	// Try to add one unit of ItemData. Stackable Items increment
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool TryAddItem(UInventoryItemData* ItemData);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool UseItem(UInventoryItemData* ItemData);

	// Removes item from inventory
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool DropItem(UInventoryItemData* ItemData);

	// Empty the entire inventory
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void Clear();

private:
	UPROPERTY()
	TArray<FInventorySlot> Slots;
};