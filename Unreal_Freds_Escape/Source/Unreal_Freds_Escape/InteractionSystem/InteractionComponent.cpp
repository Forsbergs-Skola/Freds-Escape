#include "InteractionComponent.h"
#include "Unreal_Freds_EscapeCameraManager.h"
#include "IInteractable.h"
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
}

void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

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
            FHitResult Hit;
            FCollisionQueryParams Params;
            Params.AddIgnoredActor(GetOwner());

            if (GetWorld()->SweepSingleByChannel(Hit, Start, End, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(15.f), Params))
            {
                bool bHasInterface = Hit.GetActor()->GetClass()->ImplementsInterface(UInteractable::StaticClass());
                if (Hit.GetActor() && bHasInterface)
                {
                    FocusedActor = Hit.GetActor();
                }
                else
                {
                    FocusedActor = nullptr;
                }
            }
            else
            {
                FocusedActor = nullptr;
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
    if (bIsInspecting)
    {
        // Drop item
        if (UPrimitiveComponent* Prim = Cast<UPrimitiveComponent>(HeldActor->GetRootComponent()))
        {
            Prim->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        }
        HeldActor->SetActorTransform(OriginalTransform);
        HeldActor = nullptr;
        bIsInspecting = false;
        bIsRotating = false;

        if (ACharacter* Char = Cast<ACharacter>(GetOwner()))
            Char->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
        return;
    }

    if (!FocusedActor) return;

    bool bAccepted = IInteractable::Execute_OnInteract(FocusedActor, OwnerController);
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