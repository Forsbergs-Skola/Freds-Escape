#pragma once

#include "CoreMinimal.h"
#include "Sound/SoundBase.h"
#include "NarrativeTypes.generated.h"

/**
 * A single condition that must be true for a reaction to be eligible.
 * Put several of these on one reaction to require multiple things at once.
 */
USTRUCT(BlueprintType)
struct FNarrativeCondition
{
    GENERATED_BODY()

    // An event tag that must (or must not) already be in the narrative history.
    // Leave as None for an "always eligible" condition - useful for a fallback reaction.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Narrative")
    FName RequiredEventTag = NAME_None;

    // If true, this condition passes only when RequiredEventTag has NOT happened (yet)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Narrative")
    bool bInvertCondition = false;

    // How many times RequiredEventTag needs to have fired for this to pass.
    // Handy for "player has failed this puzzle 3 times" style conditions.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Narrative")
    int32 MinimumOccurrences = 1;
};

/**
 * One possible reaction a trigger can play: some text, a sound, and/or a request
 * to change the environment. Blueprint decides HOW to actually play these fields;
 * C++ only decides WHICH reaction (if any) is eligible right now.
 */
USTRUCT(BlueprintType)
struct FNarrativeReaction
{
    GENERATED_BODY()

    // Thought/subtitle text to show. Leave empty for no text this time.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Narrative")
    FText DisplayText;

    // Audio cue to play. Leave null for no audio this time.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Narrative")
    USoundBase* AudioCue = nullptr;

    // A free-form tag Blueprint can read to decide what environmental change to make
    // (e.g. "Lights_Flicker", "Door_Unlock_VFX"). Leave None for no environment change.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Narrative")
    FName EnvironmentChangeTag = NAME_None;

    // ALL of these must pass for this reaction to be picked. Leave empty for an
    // always-available fallback (put it last in the array).
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Narrative")
    TArray<FNarrativeCondition> Conditions;

    // Recorded into the narrative history once this reaction plays, so later triggers
    // can react to it having happened. If left as None, the owning trigger's TriggerID is used instead.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Narrative")
    FName EventTag = NAME_None;
};
