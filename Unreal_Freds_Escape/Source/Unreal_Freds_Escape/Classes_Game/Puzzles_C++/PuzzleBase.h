#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PuzzleBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPuzzleSolved);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPuzzleFailed);

// The different states a puzzle can be in
UENUM(BlueprintType)
enum class EPuzzleState : uint8
{
    Inactive    UMETA(DisplayName = "Inactive"),
    Active      UMETA(DisplayName = "Active"),
    Solved      UMETA(DisplayName = "Solved"),
    Failed      UMETA(DisplayName = "Failed")
};

// The brain for all future puzzles that handles the state machine rules
UCLASS(Abstract)
class UNREAL_FREDS_ESCAPE_API APuzzleBase : public AActor
{
    GENERATED_BODY()

public:    
    APuzzleBase();

    // The list of other puzzles to unlock when this one is solved
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle")
    TArray<APuzzleBase*> dependents;

    // Should this puzzle go back to active if the player fails
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle")
    bool resetOnFailure;

    UPROPERTY(BlueprintAssignable, Category = "Puzzle")
    FOnPuzzleSolved OnPuzzleSolvedDelegate;

    UPROPERTY(BlueprintAssignable, Category = "Puzzle")
    FOnPuzzleFailed OnPuzzleFailedDelegate;

    // Turns the puzzle on
    UFUNCTION(BlueprintCallable, Category = "Puzzle")
    virtual void Activate();

    // Safely changes the puzzle state and tells blueprints about it
    UFUNCTION(BlueprintCallable, Category = "Puzzle")
    void SetState(EPuzzleState newState);

    // Returns what state the puzzle is currently in
    UFUNCTION(BlueprintPure, Category = "Puzzle")
    EPuzzleState GetState() const { return currentState; }

protected:
    virtual void BeginPlay() override;

    // The current state of this puzzle
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Puzzle")
    EPuzzleState currentState;

    // Tells the blueprints to play sounds or animations or turn on lights when the state changes
    UFUNCTION(BlueprintImplementableEvent, Category = "Puzzle")
    void OnPuzzleStateChanged(EPuzzleState newState);

    // Called when the puzzle is beaten
    UFUNCTION(BlueprintNativeEvent, Category = "Puzzle")
    void OnSolved();

    // Called when the puzzle is failed
    UFUNCTION(BlueprintNativeEvent, Category = "Puzzle")
    void OnFailed();
};
