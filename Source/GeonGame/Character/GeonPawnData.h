// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GeonPawnData.generated.h"

/**
 * 
 */
UCLASS()
class GEONGAME_API UGeonPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UGeonPawnData(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Pawn�� Class
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Geon|Pawn")
	TSubclassOf<APawn> PawnClass;
};
