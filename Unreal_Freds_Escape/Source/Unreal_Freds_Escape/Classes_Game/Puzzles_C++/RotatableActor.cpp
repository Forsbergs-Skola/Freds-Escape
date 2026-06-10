#include "RotatableActor.h"
#include "RotationPuzzle.h"

ARotatableActor::ARotatableActor()
{
    PrimaryActorTick.bCanEverTick = false; //Waiting for player to interact before the toy is active
}

void ARotatableActor::BeginPlay()
{
    Super::BeginPlay();
}


//custom function built for Blueprints use
// When Someone sets up the Blueprint so the player can spin the toy,
// they will call this function the moment the spin finishes.
void ARotatableActor::NotifyRotationChanged()
{

    if (owningPuzzle)
    {
        owningPuzzle->CheckSolution();
    }
}
