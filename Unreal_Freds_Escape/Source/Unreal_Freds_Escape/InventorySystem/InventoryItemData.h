// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/Texture2D.h"
#include "InventoryItemData.generated.h"

/**
 * A data asset that describes a single inventory item type.
 * Create one asset per item in the Content Browser and assign it
 * on the actor's IPickupable::GetItemData override in Blueprint.
 */

UCLASS(BlueprintType)
class UNREAL_FREDS_ESCAPE_API UInventoryItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	// Display name shown in the inventory slot and tooltip
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FText DisplayName;

	// Description shown in the detail/tooltip panel
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FText Description;

	// Icon texture for the inventory grid slot
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	UTexture2D* Icon;
};
