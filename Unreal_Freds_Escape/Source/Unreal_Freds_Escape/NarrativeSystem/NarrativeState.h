#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "NarrativeTypes.h"
#include "NarrativeState.generated.h"

/**
 * Tracks everything that has happened narratively during this playthrough:
 * which event tags have fired, how many times, and in what order.
 * Triggers ask this before deciding which reaction to play.
 *
 * Lives as a GameInstanceSubsystem so it persists across level loads but resets
 * on a fresh game session - call ResetState() manually if you need to test a
 * section from scratch without restarting the whole game.
 */
UCLASS()
class UNREAL_FREDS_ESCAPE_API UNarrativeState : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    // Records that an event happened. Safe to call as often as needed.
    UFUNCTION(BlueprintCallable, Category = "Narrative")
    void RecordEvent(FName EventTag);

    // Has this event tag fired at least once?
    UFUNCTION(BlueprintPure, Category = "Narrative")
    bool HasEventOccurred(FName EventTag) const;

    // How many times has this event tag fired?
    UFUNCTION(BlueprintPure, Category = "Narrative")
    int32 GetEventCount(FName EventTag) const;

    // The full ordered list of events fired so far, oldest first
    UFUNCTION(BlueprintPure, Category = "Narrative")
    const TArray<FName>& GetEventHistory() const { return EventHistory; }

    // Checks a single condition against the current state
    UFUNCTION(BlueprintPure, Category = "Narrative")
    bool EvaluateCondition(const FNarrativeCondition& Condition) const;

    // Checks that every condition in the array passes
    UFUNCTION(BlueprintPure, Category = "Narrative")
    bool EvaluateConditions(const TArray<FNarrativeCondition>& Conditions) const;

    // Wipes all recorded state. Useful for testing "play the same section twice".
    UFUNCTION(BlueprintCallable, Category = "Narrative")
    void ResetState();

protected:
    UPROPERTY()
    TMap<FName, int32> EventCounts;

    UPROPERTY()
    TArray<FName> EventHistory;
};
