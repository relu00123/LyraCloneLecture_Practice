// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GeonPlayerState.generated.h"


class UGeonPawnData;
class UGeonExperienceDefinition;
/**
 * 
 */
UCLASS()
class GEONGAME_API AGeonPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	// AActor's interface
	virtual void PostInitializeComponents() final;

	// member methods
	void OnExperienceLoaded(const UGeonExperienceDefinition* CurrentExperience);

	UPROPERTY()
	TObjectPtr<const UGeonPawnData> PawnData;	

};
