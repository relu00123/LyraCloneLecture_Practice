// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GeonCameraMode.h"
#include "GeonCameraMode_ThirdPerson.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class GEONGAME_API UGeonCameraMode_ThirdPerson : public UGeonCameraMode
{
	GENERATED_BODY()

	UGeonCameraMode_ThirdPerson(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	
};
