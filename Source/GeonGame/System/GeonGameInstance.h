// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GeonGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class GEONGAME_API UGeonGameInstance : public UGameInstance
{
	GENERATED_BODY()

	virtual void Init() override;
	virtual void Shutdown() override; 
	
};
