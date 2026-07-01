#pragma once

#include "CoreMinimal.h"
#include "NarrativeTrigger.h"
#include "InteractionSystem/IPressedInteractable.h"
#include "NarrativeInteractTrigger.generated.h"

// Fires a narrative reaction when the player interacts with (presses) this actor.
// Same interaction pattern as AKeypadPuzzle - drop this on any object the player can click/press.
UCLASS()
class UNREAL_FREDS_ESCAPE_API ANarrativeInteractTrigger : public ANarrativeTrigger, public IPressedInteractable
{
    GENERATED_BODY()

public:
    virtual FVector OnPressedFocus_Implementation(APlayerController* Player) override;
    virtual void OnButtonPressed_Implementation(APlayerController* Player, UPrimitiveComponent* HitComponent) override;
};
