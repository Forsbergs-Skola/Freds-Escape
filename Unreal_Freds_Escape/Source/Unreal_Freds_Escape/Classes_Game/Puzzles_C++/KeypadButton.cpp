#include "KeypadButton.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"

AKeypadButton::AKeypadButton()
{
    PrimaryActorTick.bCanEverTick = false;

    // Create the mesh component and make it the root
    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
    RootComponent = MeshComp;

    // Create the trigger box to detect overlaps
    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    TriggerBox->SetupAttachment(RootComponent);
    
    // Default box extent, can be scaled in blueprint
    TriggerBox->SetBoxExtent(FVector(50.f, 50.f, 50.f));

    // Initialize defaults
    DigitValue = 0;
    PressCooldown = 1.0f; // 1 second cooldown by default
    bCanBePressed = true;
}

void AKeypadButton::BeginPlay()
{
    Super::BeginPlay();

    // Bind the overlap event
    if (TriggerBox)
    {
        TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AKeypadButton::OnOverlapBegin);
    }
}

void AKeypadButton::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
                                   bool bFromSweep, const FHitResult& SweepResult)
{
    // Check if we can be pressed and the other actor is valid
    if (bCanBePressed && OtherActor && OtherActor != this)
    {
        // Simple check: Did a character (like the player) step on it?
        // You could also cast to your specific player character class if needed.
        if (OtherActor->IsA(ACharacter::StaticClass()))
        {
            // Broadcast the event to anyone listening (e.g. the KeypadPuzzle manager)
            OnButtonPressed.Broadcast(DigitValue);

            // Put the button on cooldown
            bCanBePressed = false;

            // Set a timer to reset the cooldown
            FTimerHandle TimerHandle;
            GetWorldTimerManager().SetTimer(TimerHandle, this, &AKeypadButton::ResetCooldown, PressCooldown, false);
        }
    }
}

void AKeypadButton::ResetCooldown()
{
    bCanBePressed = true;
}
