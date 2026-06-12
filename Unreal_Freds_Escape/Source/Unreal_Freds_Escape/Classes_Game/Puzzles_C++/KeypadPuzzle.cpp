#include "KeypadPuzzle.h"

AKeypadPuzzle::AKeypadPuzzle()
{
    // Initialize properties if needed
}

void AKeypadPuzzle::EnterDigit(int32 Digit)
{
    // Don't do anything if we already solved it
    if (GetState() == EPuzzleState::Solved)
    {
        return;
    }

    CurrentInput.Add(Digit);

    // Auto-check if we reached the required length
    if (CurrentInput.Num() == CorrectCode.Num())
    {
        CheckCode();
    }
    // If the player typed more digits than the correct code length, 
    // maybe they messed up and kept typing. We can auto clear or check.
    else if (CurrentInput.Num() > CorrectCode.Num())
    {
        ClearInput();
        CurrentInput.Add(Digit);
    }
}

void AKeypadPuzzle::CheckCode()
{
    // Check if the input perfectly matches the correct code
    if (CurrentInput == CorrectCode)
    {
        SetState(EPuzzleState::Solved);
    }
    else
    {
        SetState(EPuzzleState::Failed);
        ClearInput();
    }
}

void AKeypadPuzzle::ClearInput()
{
    CurrentInput.Empty();
}
