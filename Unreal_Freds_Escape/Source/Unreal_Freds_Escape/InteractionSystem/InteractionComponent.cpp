#include "InteractionComponent.h"
#include "Unreal_Freds_EscapeCameraManager.h"
#include "Unreal_Freds_EscapeCharacter.h"
#include "IInteractable.h"
#include "IPressedInteractable.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/PrimitiveComponent.h"

UInteractionComponent::UInteractionComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UInteractionComponent::BeginPlay()
{
    Super::BeginPlay();
    if (APawn* Pawn = Cast<APawn>(GetOwner()))
    {
        OwnerController = Cast<APlayerController>(Pawn->GetController());
        if (OwnerController)
        {
            CachedCameraManager = Cast<AUnreal_Freds_EscapeCameraManager>(OwnerController->PlayerCameraManager);
        }
    }

    GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
        {
            bInputReady = true;
        });
}

void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (bIsViewingPressed)
    {
        TickPressedView(DeltaTime);
        return;
    }

    if (bIsInspecting)
    {
        if (UCameraComponent* Cam = GetCamera())
        {
            FVector Target = Cam->GetComponentLocation() + Cam->GetForwardVector() * InspectDistance;
            HeldActor->SetActorLocation(FMath::VInterpTo(HeldActor->GetActorLocation(), Target, DeltaTime, 12.f),
                false, nullptr, ETeleportType::TeleportPhysics);
        }
        TickRotation(DeltaTime);
        MouseDelta = FVector2D::ZeroVector;
    }
    else
    {
        if (OwnerController)
        {
            FVector Start;
            FRotator Rotation;
            OwnerController->GetPlayerViewPoint(Start, Rotation);
            FVector End = Start + Rotation.Vector() * InteractionRange;
            FCollisionQueryParams Params;
            Params.AddIgnoredActor(GetOwner());

            TArray<FHitResult> Hits;
            FocusedActor = nullptr;

            if (GetWorld()->SweepMultiByChannel(Hits, Start, End, FQuat::Identity, ECC_GameTraceChannel1, FCollisionShape::MakeSphere(15.f), Params))
            {
                for (const FHitResult& Hit : Hits)
                {
                    if (!Hit.GetActor()) continue;

                    bool bHasInterface = Hit.GetActor()->GetClass()->ImplementsInterface(UInteractable::StaticClass());
                    bool bHasPressedInterface = Hit.GetActor()->GetClass()->ImplementsInterface(UPressedInteractable::StaticClass());

                    if (bHasInterface || bHasPressedInterface)
                    {
                        FocusedActor = Hit.GetActor();
                        break;
                    }
                }
            }
        }
        TickDOF(DeltaTime, bIsInspecting);
    }

    if (!bIsInspecting && DOFAlpha > 0.f)
    {
        TickDOF(DeltaTime, false);
    }
    else if (bIsInspecting)
    {
        TickDOF(DeltaTime, true);
    }
}


void UInteractionComponent::TryInteract()
{
    if (!bInputReady) return;

    // Drop held item
    if (bIsInspecting)
    {
        if (UPrimitiveComponent* Prim = Cast<UPrimitiveComponent>(HeldActor->GetRootComponent()))
        {
            Prim->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        }
        HeldActor->SetActorLocation(OriginalTransform.GetLocation());
        if (HeldActor->GetClass()->ImplementsInterface(UPickupable::StaticClass()))
        {
            IPickupable::Execute_OnDrop(HeldActor, OwnerController);
        }
        HeldActor = nullptr;
        bIsInspecting = false;
        bIsRotating = false;
        if (ACharacter* Char = Cast<ACharacter>(GetOwner()))
            Char->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
        return;
    }

    // Exit pressed view
    if (bIsViewingPressed)
    {
        ExitPressedView();
        return;
    }

    if (!FocusedActor) return;

    // Sub-interface: pressed interactable (keypad, buttons)
    if (FocusedActor->GetClass()->ImplementsInterface(UPressedInteractable::StaticClass()))
    {
        PressedActor = FocusedActor;
        bIsViewingPressed = true;
        PressedFocusWorldLocation = IPressedInteractable::Execute_OnPressedFocus(PressedActor, OwnerController);
        if (OwnerController)
            OriginalControlRotation = OwnerController->GetControlRotation();
        if (OwnerController)
        {
            OwnerController->SetShowMouseCursor(true);
            FInputModeGameAndUI InputMode;
            InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
            InputMode.SetHideCursorDuringCapture(false);
            OwnerController->SetInputMode(InputMode);
        }
        if (ACharacter* Char = Cast<ACharacter>(GetOwner()))
            Char->GetCharacterMovement()->DisableMovement();
        return;
    }

    // Sub-interface: pickupable item
    if (FocusedActor->GetClass()->ImplementsInterface(UPickupable::StaticClass()))
    {
        bool bAccepted = IPickupable::Execute_OnPickup(FocusedActor, OwnerController);
        if (bAccepted)
        {
            HeldActor = FocusedActor;
            bIsInspecting = true;
            OriginalTransform = HeldActor->GetActorTransform();
            if (UPrimitiveComponent* Prim = Cast<UPrimitiveComponent>(HeldActor->GetRootComponent()))
            {
                Prim->SetSimulatePhysics(false);
                Prim->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            }
            if (ACharacter* Char = Cast<ACharacter>(GetOwner()))
                Char->GetCharacterMovement()->DisableMovement();
        }
        return;
    }

    // Base interface only (door, lever, etc.) � fire and forget
    IInteractable::Execute_OnInteract(FocusedActor, OwnerController);
}


void UInteractionComponent::TickRotation(float DeltaTime)
{
    if (!bIsRotating || MouseDelta.IsNearlyZero()) return;
    UCameraComponent* Cam = GetCamera();
    if (!Cam) return;

    FQuat Yaw = FQuat(Cam->GetUpVector(), FMath::DegreesToRadians(MouseDelta.X * RotationSpeed * DeltaTime));
    FQuat Pitch = FQuat(Cam->GetRightVector(), FMath::DegreesToRadians(-MouseDelta.Y * RotationSpeed * DeltaTime));
    HeldActor->SetActorRotation(Pitch * Yaw * HeldActor->GetActorQuat(), ETeleportType::TeleportPhysics);
}


void UInteractionComponent::TickDOF(float DeltaTime, bool bEnable)
{
    UCameraComponent* Cam = GetCamera();

    if (!Cam) return;

    DOFAlpha = FMath::FInterpTo(DOFAlpha, bEnable ? 1.f : 0.f, DeltaTime, DOFBlendSpeed);
    Cam->PostProcessSettings.bOverride_DepthOfFieldFocalDistance = true;
    Cam->PostProcessSettings.bOverride_DepthOfFieldFstop = true;
    Cam->PostProcessSettings.DepthOfFieldFocalDistance = FMath::Lerp(DefaultFocalDistance, InspectFocalDistance, DOFAlpha);
    Cam->PostProcessSettings.DepthOfFieldFstop = FMath::Lerp(DefaultAperture, InspectAperture, DOFAlpha);
}


void UInteractionComponent::BeginRotate() { if (bIsInspecting) bIsRotating = true; }
void UInteractionComponent::EndRotate() { bIsRotating = false; }
void UInteractionComponent::AddMouseX(float Value) { if (bIsInspecting && bIsRotating) MouseDelta.X += Value; }
void UInteractionComponent::AddMouseY(float Value) { if (bIsInspecting && bIsRotating) MouseDelta.Y += Value; }

UCameraComponent* UInteractionComponent::GetCamera() const
{
    if (ACharacter* Char = Cast<ACharacter>(GetOwner()))
    {
        return Char->FindComponentByClass<UCameraComponent>();
    }
    return nullptr;
}


void UInteractionComponent::TickPressedView(float DeltaTime)
{
	if (!OwnerController || !PressedActor) return;

	// Check for left mouse click 
    if (OwnerController->IsInputKeyDown(EKeys::LeftMouseButton))
    {
        if (!bWasMouseDownLastFrame)
        {
            TryPressingButton();
        }
        bWasMouseDownLastFrame = true;
    }
    else
    {
        bWasMouseDownLastFrame = false;
    }

	UCameraComponent* Cam = GetCamera();

    if (Cam)
    {
		FVector CamLoc = Cam->GetComponentLocation();
		FRotator TargetRot = (PressedFocusWorldLocation - CamLoc).Rotation();
		FRotator Current = OwnerController->GetControlRotation();
		FRotator Smoothed = FMath::RInterpTo(Current, TargetRot, DeltaTime, PressedFocusSpeed);
		OwnerController->SetControlRotation(Smoothed);
    }

    TickPressedDOF(DeltaTime, true);
}


void UInteractionComponent::TickPressedDOF(float DeltaTime, bool bEnable)
{
	UCameraComponent* Cam = GetCamera();

    if (!Cam) return;

    PressedDOFAlpha = FMath::FInterpTo(PressedDOFAlpha, bEnable ? 1.f : 0.f, DeltaTime, DOFBlendSpeed);    Cam->PostProcessSettings.bOverride_DepthOfFieldFocalDistance = true;
    Cam->PostProcessSettings.bOverride_DepthOfFieldFstop = true;
    Cam->PostProcessSettings.DepthOfFieldFocalDistance = FMath::Lerp(DefaultFocalDistance, PressedFocalDistance, PressedDOFAlpha);
	Cam->PostProcessSettings.DepthOfFieldFstop = FMath::Lerp(DefaultAperture, PressedAperture, PressedDOFAlpha);
}


void UInteractionComponent::TryPressingButton()
{
	if (!bIsViewingPressed || !OwnerController) return;

    // Raycast from mouse cursor into the keypad actor's geometry
    FVector Start, WorldDirection;
    if (!OwnerController->DeprojectMousePositionToWorld(Start, WorldDirection)) return;

    FVector End = Start + (WorldDirection * InteractionRange);

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(GetOwner());

    if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
    {
        // Only fire if we hit the pressed actor itself
        if (Hit.GetActor() == PressedActor)
        {
			IPressedInteractable::Execute_OnButtonPressed(PressedActor, OwnerController, Cast<UPrimitiveComponent>(Hit.GetComponent()));
        }
    }
}


void UInteractionComponent::ExitPressedView()
{
    if (!bIsViewingPressed) return;

    if (PressedActor)
    {
        IPressedInteractable::Execute_OnPressedUnfocus(PressedActor, OwnerController);
    }

    // Restore control rotation so the camera doesn't snap
    if (OwnerController)
    {
        OwnerController->SetControlRotation(OriginalControlRotation);
    }

    if (OwnerController)
    {
        OwnerController->SetShowMouseCursor(false);
        OwnerController->SetInputMode(FInputModeGameOnly());
    }

    //Blend DOF back out
    TickPressedDOF(0.f, false);
    PressedDOFAlpha = 0.f;
    UCameraComponent* Cam = GetCamera();
    
    if (Cam)
    {
        Cam->PostProcessSettings.DepthOfFieldFocalDistance = DefaultFocalDistance;
        Cam->PostProcessSettings.DepthOfFieldFstop = DefaultAperture;
    }

    PressedActor = nullptr;
    bIsViewingPressed = false;

    if (ACharacter* Char = Cast<ACharacter>(GetOwner()))
        Char->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}


void UInteractionComponent::StoreHeldItem()
{
    if (!bIsInspecting || !HeldActor) return;

    bool bStored = false;

    if (AUnreal_Freds_EscapeCharacter* Char = Cast<AUnreal_Freds_EscapeCharacter>(GetOwner()))
    {
        if (UInventoryComponent* Inv = Char->GetInventoryComponent())
        {
            if (HeldActor->GetClass()->ImplementsInterface(UPickupable::StaticClass()))
                ItemData = IPickupable::Execute_GetItemData(HeldActor);

            bStored = Inv->TryAddItem(ItemData);
        }
    }

    if (!bStored)
    {
        // Inventory full (or no inventory component) — keep the item in the world.
        return;
    }

    HeldActor->Destroy();
    HeldActor = nullptr;
    bIsInspecting = false;
    bIsRotating = false;
    OnItemStored.Broadcast();

    if (ACharacter* Char = Cast<ACharacter>(GetOwner()))
        Char->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}
