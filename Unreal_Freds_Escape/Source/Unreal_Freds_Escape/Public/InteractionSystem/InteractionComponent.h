// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractionComponent.generated.h"


class AUnreal_Freds_EscapeCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFocusChanged, AActor*, NewFocusActor, FText, PromptText);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFocusLost, AActor*, PreviousFocusActor);


UCLASS(ClassGroup = (Interaction), meta = (BlueprintSpawnableComponent))
class UNREAL_FREDS_ESCAPE_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInteractionComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|Trace")
};