// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GeonGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class GEONGAME_API AGeonGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGeonGameModeBase(); 
	
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	/*
	* member methods
	*/
	void HandleMatchAssignmentIfNotExpectingOne();

};
