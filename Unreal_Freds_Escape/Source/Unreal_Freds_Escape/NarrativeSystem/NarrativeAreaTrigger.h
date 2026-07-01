#pragma once

#include "CoreMinimal.h"
#include "NarrativeTrigger.h"
#include "NarrativeAreaTrigger.generated.h"

class UBoxComponent;

// Fires a narrative reaction when the player enters a volume placed in the level
UCLASS()
class UNREAL_FREDS_ESCAPE_API ANarrativeAreaTrigger : public ANarrativeTrigger
{
    GENERATED_BODY()

public:
    ANarrativeAreaTrigger();

    // If true, walking through this volume again won't fire it a second time
    UPROPERTY(EditAnywhere, Category = "Narrative")
    bool bOnlyFireOnce = false;

protected:
    UPROPERTY(VisibleAnywhere, Category = "Narrative")
    TObjectPtr<UBoxComponent> TriggerVolume;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    bool bHasFired = false;
};
