// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ModularPlayerController.h"
#include "GeonPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class GEONGAME_API AGeonPlayerController : public AModularPlayerController
{
	GENERATED_BODY()

public:
	AGeonPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	
};
