
#include "InventorySystem/InventoryComponent.h"


UInventoryComponent::UInventoryComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}


bool UInventoryComponent::HasRoom() const
{
    return Slots.Num() < MaxSlots;
}


bool UInventoryComponent::HasItem(UInventoryItemData* ItemData) const
{
    if (!ItemData) return false;

    for (const FInventorySlot& Slot : Slots)
        if (Slot.ItemData == ItemData) return true;
    return false;
}


bool UInventoryComponent::TryAddItem(UInventoryItemData* ItemData)
{
    if (!ItemData) return false;

    if (AllowedItems.Num() > 0 && !AllowedItems.Contains(ItemData))
        return false;

    if (!HasRoom()) return false;

    FInventorySlot NewSlot;
    NewSlot.ItemData = ItemData;
    Slots.Add(NewSlot);

    OnInventoryChanged.Broadcast();
    return true;
}


bool UInventoryComponent::UseItem(UInventoryItemData* ItemData)
{
    if (!ItemData) return false;

    for (int32 i = 0; i < Slots.Num(); ++i)
    {
        if (Slots[i].ItemData == ItemData)
        {
            Slots.RemoveAt(i);
            OnItemUsed.Broadcast(ItemData);
            OnInventoryChanged.Broadcast();
            return true;
        }
    }
    return false;
}


bool UInventoryComponent::DropItem(UInventoryItemData* ItemData)
{
    if (!ItemData) return false;

    for (int32 i = 0; i < Slots.Num(); ++i)
    {
        if (Slots[i].ItemData == ItemData)
        {
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