#pragma once

#include "CoreMinimal.h"
#include "NarrativeTrigger.h"
#include "NarrativePuzzleTrigger.generated.h"

class APuzzleBase;

// Fires a narrative reaction when a specific puzzle is solved or failed.
// Uses the inherited PossibleReactions for the Solved case, and FailureReactions
// below for the Failed case, so you can react differently to success vs. failure.
//
// Requires PuzzleBase to broadcast OnPuzzleSolvedDelegate / OnPuzzleFailedDelegate
// (small addition to PuzzleBase.h/.cpp - see chat).
UCLASS()
class UNREAL_FREDS_ESCAPE_API ANarrativePuzzleTrigger : public ANarrativeTrigger
{
    GENERATED_BODY()

public:
    // The puzzle this trigger listens to
    UPROPERTY(EditInstanceOnly, Category = "Narrative")
    TObjectPtr<APuzzleBase> WatchedPuzzle;

    // Reactions to consider when the watched puzzle is Failed instead of Solved
    UPROPERTY(EditAnywhere, Category = "Narrative")
    TArray<FNarrativeReaction> FailureReactions;

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void HandlePuzzleSolved();

    UFUNCTION()
    void HandlePuzzleFailed();
};
