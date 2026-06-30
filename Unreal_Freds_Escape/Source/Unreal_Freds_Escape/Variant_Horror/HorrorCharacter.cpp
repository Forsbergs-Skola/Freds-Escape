

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

	// Initialize the walk speed
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AHorrorCharacter::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
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
	UE_LOG(LogTemp, Warning, TEXT("[Horror Input] Sprint Pressed! DoStartSprint called"));
	
	// Set the sprinting flag
	bSprinting = true;

	// Set the sprint walk speed
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;

	// Call the sprint state changed delegate (for crosshair or other UI)
	OnSprintStateChanged.Broadcast(true);
}

void AHorrorCharacter::DoEndSprint()
{
	UE_LOG(LogTemp, Warning, TEXT("[Horror Input] Sprint Released! DoEndSprint called"));

	// Set the sprinting flag
	bSprinting = false;

	// Set the default walk speed
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	// Call the sprint state changed delegate
	OnSprintStateChanged.Broadcast(false);
}
