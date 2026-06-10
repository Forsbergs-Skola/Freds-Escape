#include "PuzzleBase.h"
//Brain or core of the stateMachine


APuzzleBase::APuzzleBase() //Rules of all puzzles, on start all puzzles are Inactive
{
    PrimaryActorTick.bCanEverTick = false;
    currentState = EPuzzleState::Inactive;
    resetOnFailure = false;
}

void APuzzleBase::BeginPlay()
{
    Super::BeginPlay();
}

void APuzzleBase::Activate()
{
    // Only turn on if it is currently off
    if (currentState == EPuzzleState::Inactive)
    {
        SetState(EPuzzleState::Active);
    }
}

void APuzzleBase::SetState(EPuzzleState newState) //Checks states and changes states
{
    // Do nothing if we are already in this state, e.g can't go into Solved if already Solved
    if (currentState == newState)
    {
        return;
    }

    currentState = newState;
    OnPuzzleStateChanged(currentState);//This is to be used in Blueprints to trigger events such as framed toys eyes glowing red

    // If the player beat the puzzle
    if (currentState == EPuzzleState::Solved)
    {
        OnSolved();
        
        // Turn on all the connected puzzles in our list, this is how we will turn on the next puzzle
        for (APuzzleBase* dependent : dependents)
        {
            if (dependent)
            {
                dependent->Activate();
            }
        }
    }
    // If the player failed and we are allowed to reset(for later use like a timed Puzzle)
    else if (currentState == EPuzzleState::Failed && resetOnFailure)
    {
        SetState(EPuzzleState::Active);
    }
}

void APuzzleBase::OnSolved_Implementation()
{
    // Log that we beat it
    UE_LOG(LogTemp, Log, TEXT("Puzzle Solved: %s"), *GetName());
}
