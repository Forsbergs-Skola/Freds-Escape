

#include "Variant_Horror/HorrorCharacter.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SpotLightComponent.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"

AHorrorCharacter::AHorrorCharacter()
{
	// create the spotlight
	SpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight"));
	SpotLight->SetupAttachment(GetFirstPersonCameraComponent());

	SpotLight->SetRelativeLocationAndRotation(FVector(30.0f, 17.5f, -5.0f), FRotator(-18.6f, -1.3f, 5.26f));
	SpotLight->Intensity = 0.5;
	SpotLight->SetIntensityUnits(ELightUnits::Lumens);
	SpotLight->AttenuationRadius = 1050.0f;
	SpotLight->InnerConeAngle = 18.7f;
	SpotLight->OuterConeAngle = 45.24f;
}

void AHorrorCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Initialize sprint meter to max
	SprintMeter = SprintTime;

	// Initialize the walk speed
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	// Start the sprint tick timer
	GetWorld()->GetTimerManager().SetTimer(SprintTimer, this, &AHorrorCharacter::SprintFixedTick, SprintFixedTickTime, true);
}

void AHorrorCharacter::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// Clear the sprint timer
	GetWorld()->GetTimerManager().ClearTimer(SprintTimer);
}

void AHorrorCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UE_LOG(LogTemp, Warning, TEXT("HorrorCharacter SetupPlayerInputComponent called"));

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("EnhancedInputComponent cast succeeded"));

		if (SprintAction)
		{
			UE_LOG(LogTemp, Warning, TEXT("SprintAction is valid: %s"), *SprintAction->GetName());
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AHorrorCharacter::DoStartSprint);
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AHorrorCharacter::DoEndSprint);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("SprintAction is NULL"));
		}
	}
}

void AHorrorCharacter::DoStartSprint()
{
	UE_LOG(LogTemp, Warning, TEXT("DoStartSprint called"));
	// Set the sprinting flag
	bSprinting = true;

	// Are we out of recovery mode?
	if (!bRecovering)
	{
		// Set the sprint walk speed
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;

		// Call the sprint state changed delegate
		OnSprintStateChanged.Broadcast(true);
	}

}

void AHorrorCharacter::DoEndSprint()
{
	UE_LOG(LogTemp, Warning, TEXT("DoEndSprint called"));

	// Set the sprinting flag
	bSprinting = false;

	// Are we out of recovery mode?
	if (!bRecovering)
	{
		// Set the default walk speed
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

		// Call the sprint state changed delegate
		OnSprintStateChanged.Broadcast(false);
	}
}

void AHorrorCharacter::SprintFixedTick()
{
	if (bSprinting && !bRecovering)
	{
		// Drain stamina whenever sprint is held (not gated by velocity
		if (SprintMeter > 0.0f)
		{
			SprintMeter = FMath::Max(SprintMeter - SprintFixedTickTime, 0.0f);

			if (SprintMeter <= 0.0f)
			{
				bRecovering = true;
				GetCharacterMovement()->MaxWalkSpeed = RecoveringWalkSpeed;
				OnSprintStateChanged.Broadcast(false);
			}
		}
	}
	else
	{
		// Recover stamina when not sprinting or when recovering
		SprintMeter = FMath::Min(SprintMeter + SprintFixedTickTime, SprintTime);

		if (bRecovering && SprintMeter >= SprintTime)
		{
			bRecovering = false;
			GetCharacterMovement()->MaxWalkSpeed = bSprinting ? SprintSpeed : WalkSpeed;
			OnSprintStateChanged.Broadcast(bSprinting);
		}
	}

	OnSprintMeterUpdated.Broadcast(SprintMeter / SprintTime);
}
