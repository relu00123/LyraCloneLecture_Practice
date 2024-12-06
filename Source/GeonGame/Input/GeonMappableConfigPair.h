// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

 

#include "CoreMinimal.h"
#include "GeonMappableConfigPair.generated.h"

 /** forward declaration */
class UPlayerMappableInputConfig;

USTRUCT()
struct FGeonMappableConfigPair
{
	GENERATED_BODY()
public:
	FGeonMappableConfigPair() = default;


	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UPlayerMappableInputConfig> Config;

	UPROPERTY(EditAnywhere)
	bool bShouldActivateAutomatically = true;
};