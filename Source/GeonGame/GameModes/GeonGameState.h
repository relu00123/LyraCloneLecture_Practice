// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GeonGameState.generated.h"

class UGeonExperienceManagerComponent;
/**
 * 
 */
UCLASS()
class GEONGAME_API AGeonGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	AGeonGameState();

public:
	UPROPERTY()
	TObjectPtr<UGeonExperienceManagerComponent> ExperienceManagerComponent;
};
