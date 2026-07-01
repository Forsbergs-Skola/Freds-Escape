#include "NarrativeTrigger.h"
#include "NarrativeState.h"
#include "Engine/GameInstance.h"

ANarrativeTrigger::ANarrativeTrigger()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ANarrativeTrigger::BeginPlay()
{
    Super::BeginPlay();

    if (UGameInstance* GameInstance = GetGameInstance())
    {
        NarrativeState = GameInstance->GetSubsystem<UNarrativeState>();
    }
}

const FNarrativeReaction* ANarrativeTrigger::SelectReaction(const TArray<FNarrativeReaction>& Reactions) const
{
    if (!NarrativeState)
    {
        return nullptr;
    }

    for (const FNarrativeReaction& Reaction : Reactions)
    {
        if (NarrativeState->EvaluateConditions(Reaction.Conditions))
        {
            return &Reaction;
        }
    }
    return nullptr;
}

void ANarrativeTrigger::ResolveAndPlay(const FNarrativeReaction& Reaction)
{
    PlayReaction(Reaction);

    if (NarrativeState)
    {
        NarrativeState->RecordEvent(Reaction.EventTag != NAME_None ? Reaction.EventTag : TriggerID);
    }
}

void ANarrativeTrigger::FireTrigger()
{
    const FNarrativeReaction* Chosen = SelectReaction(PossibleReactions);
    if (Chosen)
    {
        ResolveAndPlay(*Chosen);
    }
}
