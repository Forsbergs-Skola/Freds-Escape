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
    for (const FRotatableRequirement& req : requiredRotations)
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

        // If it is rotated too far a fail check
        if (angularDiffDeg > req.toleranceDegrees)
        {
            allCorrect = false;
            break;
        }
    }

    // If we made it through the whole list without failing then we win yayy, set state to Solved
    if (allCorrect && requiredRotations.Num() > 0)
    {
        SetState(EPuzzleState::Solved);
    }
}
