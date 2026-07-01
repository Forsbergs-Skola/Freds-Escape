#include "NarrativeInteractTrigger.h"

FVector ANarrativeInteractTrigger::OnPressedFocus_Implementation(APlayerController* Player)
{
    return GetActorLocation();
}

void ANarrativeInteractTrigger::OnButtonPressed_Implementation(APlayerController* Player, UPrimitiveComponent* HitComponent)
{
    FireTrigger();
}
