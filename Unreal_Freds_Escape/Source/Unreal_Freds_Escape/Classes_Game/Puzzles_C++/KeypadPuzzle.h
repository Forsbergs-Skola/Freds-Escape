#pragma once

#include "CoreMinimal.h"
#include "PuzzleBase.h"
#include "KeypadPuzzle.generated.h"

/**
 * The manager for a keypad or code lock puzzle.
 */
UCLASS()
class UNREAL_FREDS_ESCAPE_API AKeypadPuzzle : public APuzzleBase
{
    GENERATED_BODY()

public:
    AKeypadPuzzle();

    // The correct sequence of numbers needed to solve the puzzle
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle")
    TArray<int32> CorrectCode;

    // The sequence of numbers the player has entered so far
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Puzzle")
    TArray<int32> CurrentInput;

    // Call this when the player presses a number on the keypad
    UFUNCTION(BlueprintCallable, Category = "Puzzle")
    void EnterDigit(int32 Digit);

    // Call this manually if there's an 'Enter' button, or let EnterDigit auto-call it
    UFUNCTION(BlueprintCallable, Category = "Puzzle")
    void CheckCode();

    // Call this if the player hits 'Clear' or if they enter the wrong code
    UFUNCTION(BlueprintCallable, Category = "Puzzle")
    void ClearInput();
};
