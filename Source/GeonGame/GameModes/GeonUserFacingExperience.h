// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GeonUserFacingExperience.generated.h"

class UCommonSession_HostSessionRequest;


/**
 * 
 */
UCLASS()
class GEONGAME_API UGeonUserFacingExperience : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// allowedTypes 를 지정하면 해당 Type? 만 지정할 수 있다. 

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	UCommonSession_HostSessionRequest* CreateHostingRequest() const;

	// ** the specific map to load */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Experience, meta = (AllowedTypes = "Map"))
	FPrimaryAssetId MapID; // 유저가 경험할 수 있는 맵 

	/* the gameplay experience to load */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Experience, meta = (AllowedTypes = "GeonExperienceDefinition"))
	FPrimaryAssetId ExperienceID; 
};
