#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RotatableActor.generated.h"

class ARotationPuzzle;

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

protected:
    virtual void BeginPlay() override;
};
