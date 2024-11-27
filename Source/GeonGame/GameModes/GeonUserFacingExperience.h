// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GeonUserFacingExperience.generated.h"

/**
 * 
 */
UCLASS()
class GEONGAME_API UGeonUserFacingExperience : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// allowedTypes �� �����ϸ� �ش� Type? �� ������ �� �ִ�. 

	// ** the specific map to load */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Experience, meta = (AllowedTypes = "Map"))
	FPrimaryAssetId MapID; // ������ ������ �� �ִ� �� 

	/* the gameplay experience to load */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Experience, meta = (AllowedTypes = "GeonExperienceDefinition"))
	FPrimaryAssetId ExperienceID; 
};
