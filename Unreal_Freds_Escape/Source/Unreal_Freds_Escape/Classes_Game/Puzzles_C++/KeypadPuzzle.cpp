#include "KeypadPuzzle.h"
#include "KeypadButton.h"
#include "Engine/Engine.h"

AKeypadPuzzle::AKeypadPuzzle()
{
    // Initialize properties if needed
}

void AKeypadPuzzle::BeginPlay()
{
    Super::BeginPlay();

    // Bind to all the physical buttons assigned to this puzzle
    for (AKeypadButton* Button : KeypadButtons)
    {
        if (Button)
        {
            Button->OnButtonPressed.AddDynamic(this, &AKeypadPuzzle::HandleButtonPressed);
        }
    }
}

void AKeypadPuzzle::HandleButtonPressed(int32 Digit)
{
    EnterDigit(Digit);
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
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Correct code entered!"));
        }
        SetState(EPuzzleState::Solved);
    }
    else
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Wrong code entered!"));
        }
        SetState(EPuzzleState::Failed);
        ClearInput();
    }
}

void AKeypadPuzzle::ClearInput()
{
    CurrentInput.Empty();
}
