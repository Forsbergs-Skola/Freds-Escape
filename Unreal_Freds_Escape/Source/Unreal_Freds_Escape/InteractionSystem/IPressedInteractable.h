#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IPressedInteractable.generated.h"


UINTERFACE(MinimalAPI, Blueprintable)
class UPressedInteractable : public UInterface
{
	GENERATED_BODY()
};


class UNREAL_FREDS_ESCAPE_API IPressedInteractable
{
	GENERATED_BODY()

public:

	// Called when the player firsts focuses on where to press (with E)
	UFUNCTION(BlueprintNativeEvent, Category = "Interaction|Pressed")
	FVector OnPressedFocus(APlayerController* Player);
	virtual FVector OnPressedFocus_Implementation(APlayerController* Player)
	{
		return FVector::ZeroVector;
	}

	// Called when the player left clicks on a primitive inside the button actor
	// ComponentName is the hit component's name so BP can route to the right button
	UFUNCTION(BlueprintNativeEvent, Category = "Interaction|Pressed")
	void OnButtonPressed(APlayerController* Player, UPrimitiveComponent* HitComponent);
	virtual void OnButtonPressed_Implementation(APlayerController* Player, UPrimitiveComponent* HitComponent) {}

	// Called when the player exits pressing button view with E
	UFUNCTION(BlueprintNativeEvent, Category = "Interaction|Pressed")
	void OnPressedUnfocus(APlayerController* Player);
	virtual void OnPressedUnfocus_Implementation(APlayerController* Player) {}
};