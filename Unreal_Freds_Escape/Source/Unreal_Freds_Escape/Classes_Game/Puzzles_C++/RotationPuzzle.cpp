#include "RotationPuzzle.h"
#include "Math/UnrealMathUtility.h"

ARotationPuzzle::ARotationPuzzle()
{
}

void ARotationPuzzle::CheckSolution()
{
    // Do not check anything if the puzzle is not turned on
    if (currentState != EPuzzleState::Active)
    {
        return;
    }

    bool allCorrect = true;

    // Loop through every toy on our list
    for (FRotatableRequirement& req : requiredRotations)
    {
        // Skip it if there is no actor in scene or someone forgot to connect it to plug a toy in
        if (!req.targetActor)
        {
            continue; 
        }

        FRotator currentRot = req.targetActor->GetActorRotation();
        
        // Calculate the physical angle difference using math to avoid glitches(hopefully)
        float angularDiffRads = req.targetActor->GetActorQuat().AngularDistance(req.requiredRotation.Quaternion());
        float angularDiffDeg = FMath::RadiansToDegrees(angularDiffRads);

        bool bIsCurrentlyCorrect = (angularDiffDeg <= req.toleranceDegrees);

        // Check if the state changed to correct
        if (bIsCurrentlyCorrect && !req.bWasCorrect)
        {
            req.targetActor->OnCorrectRotation.Broadcast();
        }
        // Check if the state changed to incorrect
        else if (!bIsCurrentlyCorrect && req.bWasCorrect)
        {
            req.targetActor->OnIncorrectRotation.Broadcast();
        }

        // Update the tracked state
        req.bWasCorrect = bIsCurrentlyCorrect;

        // If it is rotated too far a fail check
        if (!bIsCurrentlyCorrect)
        {
            allCorrect = false;
        }
    }

    // If we made it through the whole list without failing then we win yayy, set state to Solved
    if (allCorrect && requiredRotations.Num() > 0)
    {
        SetState(EPuzzleState::Solved);
    }
}
