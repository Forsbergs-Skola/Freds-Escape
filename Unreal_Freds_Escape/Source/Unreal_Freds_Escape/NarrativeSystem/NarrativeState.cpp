#include "NarrativeState.h"
#include "Math/UnrealMathUtility.h"

void UNarrativeState::RecordEvent(FName EventTag)
{
    if (EventTag == NAME_None)
    {
        return;
    }

    int32& Count = EventCounts.FindOrAdd(EventTag);
    Count++;
    EventHistory.Add(EventTag);
}

bool UNarrativeState::HasEventOccurred(FName EventTag) const
{
    return EventCounts.Contains(EventTag);
}

int32 UNarrativeState::GetEventCount(FName EventTag) const
{
    const int32* Count = EventCounts.Find(EventTag);
    return Count ? *Count : 0;
}

bool UNarrativeState::EvaluateCondition(const FNarrativeCondition& Condition) const
{
    // An empty condition (no tag set) always passes - this is how you make a fallback reaction
    if (Condition.RequiredEventTag == NAME_None)
    {
        return true;
    }

    const bool bOccurredEnough = GetEventCount(Condition.RequiredEventTag) >= FMath::Max(1, Condition.MinimumOccurrences);

    return Condition.bInvertCondition ? !bOccurredEnough : bOccurredEnough;
}

bool UNarrativeState::EvaluateConditions(const TArray<FNarrativeCondition>& Conditions) const
{
    for (const FNarrativeCondition& Condition : Conditions)
    {
        if (!EvaluateCondition(Condition))
        {
            return false;
        }
    }
    return true;
}

void UNarrativeState::ResetState()
{
    EventCounts.Empty();
    EventHistory.Empty();
}
