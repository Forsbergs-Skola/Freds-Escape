#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IInteractable.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UInteractable : public UInterface
{
    GENERATED_BODY()
};

class UNREAL_FREDS_ESCAPE_API IInteractable
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintNativeEvent, Category = "Interaction") 
    bool OnInteract(APlayerController* Player);
    virtual bool OnInteract_Implementation(APlayerController* Player) { return true; }
};