#pragma once

#include "CoreMinimal.h"
#include "PuzzleBase.h"
#include "RotatableActor.h"
#include "RotationPuzzle.generated.h"

// Holds the rules for where a single toy is supposed to be pointing
USTRUCT(BlueprintType)
struct FRotatableRequirement
{
    GENERATED_BODY()

    // The specific toy/Actor we are checking
    UPROPERTY(EditInstanceOnly, Category = "Puzzle")
    ARotatableActor* targetActor;

    // The exact rotation it needs to match
    UPROPERTY(EditInstanceOnly, Category = "Puzzle")
    FRotator requiredRotation;

    // The amount of error or wiggle room allowed for the player when setting Rotation
    UPROPERTY(EditInstanceOnly, Category = "Puzzle")
    float toleranceDegrees;

    // Tracks if this toy was correct last time we checked
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Puzzle")
    bool bWasCorrect;

    FRotatableRequirement()
        : targetActor(nullptr)
        , requiredRotation(FRotator::ZeroRotator)
        , toleranceDegrees(5.0f)
        , bWasCorrect(false)
    {}
};

// The manager specifically for the toy rotation puzzle
UCLASS()
class UNREAL_FREDS_ESCAPE_API ARotationPuzzle : public APuzzleBase
{
    GENERATED_BODY()

public:
    ARotationPuzzle();

    // List of toys and thier Rotation
    UPROPERTY(EditInstanceOnly, Category = "Puzzle")
    TArray<FRotatableRequirement> requiredRotations;

    // Loops through all toys to see if the puzzle is beaten
    UFUNCTION(BlueprintCallable, Category = "Puzzle")
    void CheckSolution();
};
