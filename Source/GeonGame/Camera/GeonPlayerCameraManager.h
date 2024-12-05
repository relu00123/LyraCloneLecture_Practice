// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "GeonPlayerCameraManager.generated.h"


/**
 * Controller에 바인딩된 CameraManager
 */
#define GEON_CAMERA_DEFAULT_FOV (80.0f)
#define GEON_CAMERA_DEFAULT_PITCH_MIN (-89.0f)
#define GEON_CAMERA_DEFAULT_PITCH_MAX (89.0f)

/**
 * 
 */
UCLASS()
class GEONGAME_API AGeonPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	AGeonPlayerCameraManager(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
};
