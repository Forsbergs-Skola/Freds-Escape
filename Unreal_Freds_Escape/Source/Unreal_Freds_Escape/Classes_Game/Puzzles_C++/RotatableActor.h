#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RotatableActor.generated.h"

class ARotationPuzzle;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCorrectRotationSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnIncorrectRotationSignature);

// The Toy that the player can spin
UCLASS()
class UNREAL_FREDS_ESCAPE_API ARotatableActor : public AActor
{
    GENERATED_BODY()
    
public:    
    ARotatableActor();

    // The manager that this toy reports to
    UPROPERTY(EditInstanceOnly, Category = "Puzzle")
    class ARotationPuzzle* owningPuzzle;

    // Blueprints call this to shout to the manager that it was moved
    UFUNCTION(BlueprintCallable, Category = "Puzzle")
    void NotifyRotationChanged();

    // The number that will be displayed under the frame for Puzzle 2
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle")
    int32 FrameNumber;

    // Event dispatcher whoch can be fired in Blueprints when this specific toy reaches its correct rotation
    UPROPERTY(BlueprintAssignable, Category = "Puzzle")
    FOnCorrectRotationSignature OnCorrectRotation;

    // Event dispatcher fired in Blueprints when this specific toy is rotated away from its correct rotation
    UPROPERTY(BlueprintAssignable, Category = "Puzzle")
    FOnIncorrectRotationSignature OnIncorrectRotation;

protected:
    virtual void BeginPlay() override;
};
