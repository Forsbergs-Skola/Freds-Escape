#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KeypadButton.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

// Delegate used to notify any listener that this specific button was pressed
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnKeypadButtonPressed, int32, Digit);

UCLASS()
class UNREAL_FREDS_ESCAPE_API AKeypadButton : public AActor
{
    GENERATED_BODY()
    
public:    
    AKeypadButton();

    // The visual mesh for the cube
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* MeshComp;

    // The collision box used to detect the player jumping on it
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBoxComponent* TriggerBox;

    // The number this button represents (e.g. 1-9)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Keypad Button")
    int32 DigitValue;

    // Event broadcasted when the player steps on this button
    UPROPERTY(BlueprintAssignable, Category = "Keypad Button")
    FOnKeypadButtonPressed OnButtonPressed;

    // Cooldown duration to prevent the button from being pressed multiple times per second
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Keypad Button")
    float PressCooldown;

protected:
    virtual void BeginPlay() override;

    // Called when something enters the TriggerBox
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
                        bool bFromSweep, const FHitResult& SweepResult);

private:
    bool bCanBePressed;
    
    // Resets the cooldown allowing the button to be pressed again
    void ResetCooldown();
};
