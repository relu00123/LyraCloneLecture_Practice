// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GeonMappableConfigPair.generated.h"

#include "CoreMinimal.h"

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