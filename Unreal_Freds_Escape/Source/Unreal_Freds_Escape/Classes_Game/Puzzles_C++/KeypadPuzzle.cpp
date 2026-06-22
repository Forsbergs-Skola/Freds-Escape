#include "KeypadPuzzle.h"
#include "Engine/Engine.h"
#include "Components/PrimitiveComponent.h"

AKeypadPuzzle::AKeypadPuzzle()
{
    // Initialize properties if needed
}

FVector AKeypadPuzzle::OnPressedFocus_Implementation(APlayerController* Player)
{
    // Return a location slightly in front of the keypad so the camera zooms in
    return GetActorLocation() + (GetActorForwardVector() * 50.f);
}

void AKeypadPuzzle::OnButtonPressed_Implementation(APlayerController* Player, UPrimitiveComponent* HitComponent)
{
    if (!HitComponent) return;

    if (HitComponent->ComponentTags.Contains(FName("Digit_1"))) EnterDigit(1);
    else if (HitComponent->ComponentTags.Contains(FName("Digit_2"))) EnterDigit(2);
    else if (HitComponent->ComponentTags.Contains(FName("Digit_3"))) EnterDigit(3);
    else if (HitComponent->ComponentTags.Contains(FName("Digit_4"))) EnterDigit(4);
    else if (HitComponent->ComponentTags.Contains(FName("Digit_5"))) EnterDigit(5);
    else if (HitComponent->ComponentTags.Contains(FName("Digit_6"))) EnterDigit(6);
    else if (HitComponent->ComponentTags.Contains(FName("Digit_7"))) EnterDigit(7);
    else if (HitComponent->ComponentTags.Contains(FName("Digit_8"))) EnterDigit(8);
    else if (HitComponent->ComponentTags.Contains(FName("Digit_9"))) EnterDigit(9);
    else if (HitComponent->ComponentTags.Contains(FName("Digit_0"))) EnterDigit(0);
    else if (HitComponent->ComponentTags.Contains(FName("Clear"))) ClearInput();
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
