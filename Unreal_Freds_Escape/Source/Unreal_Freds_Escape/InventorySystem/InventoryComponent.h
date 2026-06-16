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

	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	int32 Quantity = 0;

	bool IsEmpty() const
	{
		return ItemData == nullptr || Quantity <= 0;
	}
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryChanged);


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
	int32 MaxSlots = 20;

	//Broadcast whenever the inventory changes and bind this in WBP_Inventory
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryChanged OnInventoryChanged;

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

	// Remove one unit of ItemData
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveItem(UInventoryItemData* ItemData);

	// Empty the entire inventory
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void Clear();

private:
	UPROPERTY()
	TArray<FInventorySlot> Slots;
};