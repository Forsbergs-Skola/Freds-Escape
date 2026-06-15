// Copyright Epic Games, Inc. All Rights Reserved.


#pragma once
#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "Unreal_Freds_EscapeCameraManager.generated.h"

/**
 *  Basic First Person camera manager.
 *  Limits min/max look pitch.
 */

UCLASS()
class AUnreal_Freds_EscapeCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	/** Constructor */
	AUnreal_Freds_EscapeCameraManager();
};