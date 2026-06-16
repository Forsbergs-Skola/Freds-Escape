#include "InventorySystem/InventoryComponent.h"


UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


bool UInventoryComponent::HasRoom() const
{
	int32 Used = 0;

	for (const FInventorySlot& Slot : Slots)
		if(!Slot.IsEmpty()) ++Used;
	return Used < MaxSlots;
}


bool UInventoryComponent::HasItem(UInventoryItemData* ItemData) const
{
	if (!ItemData) return false; 

	for (const FInventorySlot& Slot : Slots)
		if (Slot.ItemData == ItemData && Slot.Quantity > 0) return true;
	return false;
}


bool UInventoryComponent::TryAddItem(UInventoryItemData* ItemData)
{
	if (!ItemData) return false;

	if (ItemData->bIsStackable)
	{
		for (FInventorySlot& Slot : Slots)
		{
			if (Slot.ItemData == ItemData && Slot.Quantity < ItemData->MaxStackSize)
			{
				++Slot.Quantity;
				OnInventoryChanged.Broadcast();
				return true;
			}
		}
	}

	if (!HasRoom()) return false;

	FInventorySlot NewSlot;
	NewSlot.ItemData = ItemData;
	NewSlot.Quantity = 1;
	Slots.Add(NewSlot);

	OnInventoryChanged.Broadcast();
	return true;
}


bool UInventoryComponent::RemoveItem(UInventoryItemData* ItemData)
{
	if (!ItemData) return false;

	for (int32 i = 0; i < Slots.Num(); ++i)
	{
		if (Slots[i].ItemData == ItemData)
		{
			--Slots[i].Quantity;

			if (Slots[i].Quantity <= 0)
				Slots.RemoveAt(i);

			OnInventoryChanged.Broadcast();
			return true;
		}
	}

	return false;
}


void UInventoryComponent::Clear()
{
	Slots.Empty();
	OnInventoryChanged.Broadcast();
}