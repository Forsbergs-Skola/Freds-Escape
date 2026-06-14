// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"


class UCameraComponent;
class AUnreal_Freds_EscapeCameraManager;


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
	FVector2D MouseDelta = FVector2D::ZeroVector;
	FTransform OriginalTransform;
	float DOFAlpha = 0.f;

	UCameraComponent* GetCamera() const;
	void TickRotation(float DeltaTime);
	void TickDOF(float DeltaTime, bool bEnable);
};