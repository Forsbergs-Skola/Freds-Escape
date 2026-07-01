#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NarrativeTypes.h"
#include "NarrativeTrigger.generated.h"

class UNarrativeState;

/**
 * Base for anything that can cause a narrative reaction.
 * C++ decides WHICH reaction is eligible based on what has happened so far
 * (via UNarrativeState); Blueprint decides HOW to actually play it - spawn a
 * thought-text widget, play the audio cue, react to the environment tag, etc.
 *
 * Don't use this directly - use ANarrativeAreaTrigger (enter a space),
 * ANarrativeInteractTrigger (press/interact with an object), or
 * ANarrativePuzzleTrigger (a puzzle was solved or failed). You can also
 * subclass this yourself and call FireTrigger() from anywhere.
 */
UCLASS(Abstract, Blueprintable)
class UNREAL_FREDS_ESCAPE_API ANarrativeTrigger : public AActor
{
    GENERATED_BODY()

public:
    ANarrativeTrigger();

    // Identifies this trigger in the narrative history (used when a reaction doesn't set its own EventTag)
    UPROPERTY(EditAnywhere, Category = "Narrative")
    FName TriggerID;

    // Reactions are checked in order - put your most specific reactions first,
    // and a reaction with no conditions last as a fallback.
    UPROPERTY(EditAnywhere, Category = "Narrative")
    TArray<FNarrativeReaction> PossibleReactions;

    // Picks the first eligible reaction from PossibleReactions and plays it. Call this
    // from Blueprint, from an overlap event, from an interact callback, wherever fits.
    UFUNCTION(BlueprintCallable, Category = "Narrative")
    virtual void FireTrigger();

protected:
    virtual void BeginPlay() override;

    // Finds the first reaction in Reactions whose conditions all currently pass, or nullptr if none do
    const FNarrativeReaction* SelectReaction(const TArray<FNarrativeReaction>& Reactions) const;

    // Records the reaction (and this trigger's ID) to history, then asks Blueprint to play it
    void ResolveAndPlay(const FNarrativeReaction& Reaction);

    // Blueprint implements the actual presentation: show DisplayText, play AudioCue,
    // and respond to EnvironmentChangeTag however this trigger needs to.
    UFUNCTION(BlueprintImplementableEvent, Category = "Narrative")
    void PlayReaction(const FNarrativeReaction& Reaction);

    UPROPERTY(BlueprintReadOnly, Category = "Narrative")
    TObjectPtr<UNarrativeState> NarrativeState;
};
