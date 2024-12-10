// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GeonExperienceActionSet.generated.h"

class UGameFeatureAction;
/**
 * 
 */
UCLASS()
class GEONGAME_API UGeonExperienceActionSet : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UGeonExperienceActionSet();


	//member variables
	UPROPERTY(EditAnywhere, Category = "Actions to Perform")
	TArray<TObjectPtr<UGameFeatureAction>> Actions;

};
