#include "NarrativeAreaTrigger.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"

ANarrativeAreaTrigger::ANarrativeAreaTrigger()
{
    TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
    TriggerVolume->SetBoxExtent(FVector(100.f, 100.f, 100.f));
    TriggerVolume->SetCollisionProfileName(TEXT("Trigger"));
    RootComponent = TriggerVolume;

    TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &ANarrativeAreaTrigger::OnOverlapBegin);
}

void ANarrativeAreaTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // Only react to the player character
    if (!OtherActor || !OtherActor->IsA<ACharacter>())
    {
        return;
    }

    if (bOnlyFireOnce && bHasFired)
    {
        return;
    }

    bHasFired = true;
    FireTrigger();
}
