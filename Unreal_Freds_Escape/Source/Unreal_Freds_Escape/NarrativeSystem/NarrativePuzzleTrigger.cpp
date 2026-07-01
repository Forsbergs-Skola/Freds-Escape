#include "NarrativePuzzleTrigger.h"
#include "Classes_Game/Puzzles_C++/PuzzleBase.h"

void ANarrativePuzzleTrigger::BeginPlay()
{
    Super::BeginPlay();

    if (WatchedPuzzle)
    {
        WatchedPuzzle->OnPuzzleSolvedDelegate.AddDynamic(this, &ANarrativePuzzleTrigger::HandlePuzzleSolved);
        WatchedPuzzle->OnPuzzleFailedDelegate.AddDynamic(this, &ANarrativePuzzleTrigger::HandlePuzzleFailed);
    }
}

void ANarrativePuzzleTrigger::HandlePuzzleSolved()
{
    // Uses the inherited PossibleReactions
    FireTrigger();
}

void ANarrativePuzzleTrigger::HandlePuzzleFailed()
{
    const FNarrativeReaction* Chosen = SelectReaction(FailureReactions);
    if (Chosen)
    {
        ResolveAndPlay(*Chosen);
    }
}
