// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GeonGameModeBase.generated.h"

class UGeonPawnData;
class UGeonExperienceDefinition;
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

	virtual void InitGameState() final;

	/** GetDefaultPawnClassForController */
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) final;

	/** HandleStartingNewPlayer */
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) final;

	/** SpawnDefaultPawnAtTransform */
	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) final;

	/*
	* member methods
	*/
	void HandleMatchAssignmentIfNotExpectingOne();
	void OnMatchAssignmentGiven(FPrimaryAssetId ExperienceId);
	bool IsExperienceLoaded() const; 
	void OnExperienceLoaded(const UGeonExperienceDefinition* CurrentExperience);
	const UGeonPawnData* GetPawnDataForController(const AController* InController) const;

};
