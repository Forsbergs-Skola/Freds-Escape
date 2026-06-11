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
    ARotationPuzzle* owningPuzzle;

    // Blueprints call this to shout to the manager that it was moved
    UFUNCTION(BlueprintCallable, Category = "Puzzle")
    void NotifyRotationChanged();

protected:
    virtual void BeginPlay() override;
};
