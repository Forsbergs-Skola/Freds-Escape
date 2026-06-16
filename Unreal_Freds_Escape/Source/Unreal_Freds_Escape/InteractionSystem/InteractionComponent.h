// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnhancedInputComponent.h"
#include "InventorySystem/IPickupable.h"
#include "InventorySystem/InventoryComponent.h"
#include "InteractionComponent.generated.h"


class UCameraComponent;
class AUnreal_Freds_EscapeCameraManager;
class IPressedInteractable;


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnItemStored);


UCLASS(ClassGroup = (Interaction), meta = (BlueprintSpawnableComponent))
class UNREAL_FREDS_ESCAPE_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInteractionComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float InteractionRange = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float InspectDistance = 60.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float RotationSpeed = 150.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|DOF")
	float InspectFocalDistance = 60.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|DOF")
	float DOFBlendSpeed = 6.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|DOF")
	float InspectAperture = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|DOF")
	float DefaultFocalDistance = 10000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|DOF")
	float DefaultAperture = 32.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	bool bIsInspecting = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|Pressed") // Distance from camera the pressed view point is held at in cm
	float PressedViewDistance = 80.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|Pressed") // How fast the camera arm lerps towards the pressed focus point
	float PressedFocusSpeed = 8.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|Pressed") // DOF focal distance when looking at buttons
	float PressedFocalDistance = 80.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|Pressed") // Aperture while viewing buttons
	float PressedAperture = 1.4;

	UPROPERTY(BlueprintReadOnly, Category = "Interaction|Pressed")
	bool bIsViewingPressed = false;

	UPROPERTY(BlueprintAssignable, Category = "Interaction|Inventory")
	FOnItemStored OnItemStored;

	UPROPERTY()
	UInventoryItemData* ItemData = nullptr;

	UPROPERTY()
	AUnreal_Freds_EscapeCameraManager* CachedCameraManager = nullptr;

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void TryInteract();

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void BeginRotate();

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void EndRotate();

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void AddMouseX(float Value);

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void AddMouseY(float Value);

	UFUNCTION(BlueprintCallable, Category = "Interaction|Pressed")
	void TryPressingButton();

	UFUNCTION(BlueprintCallable, Category = "Interaction|Inventory")
	void StoreHeldItem();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


private:
	UPROPERTY()
	AActor* HeldActor = nullptr;

	UPROPERTY()
	AActor* FocusedActor = nullptr;

	UPROPERTY()
	APlayerController* OwnerController = nullptr;

	bool bIsRotating = false;
	bool bInputReady = false;
	bool bWasMouseDownLastFrame = false;
	FVector2D MouseDelta = FVector2D::ZeroVector;
	FTransform OriginalTransform;
	float DOFAlpha = 0.f;

	UPROPERTY()
	AActor* PressedActor = nullptr;
	FVector PressedFocusWorldLocation = FVector::ZeroVector;
	FRotator OriginalControlRotation = FRotator::ZeroRotator;
	float PressedDOFAlpha = 0.f;

	void TickPressedView(float DeltaTime);
	void TickPressedDOF(float DeltaTime, bool bEnable);
	void ExitPressedView();

	UCameraComponent* GetCamera() const;
	void TickRotation(float DeltaTime);
	void TickDOF(float DeltaTime, bool bEnable);
};