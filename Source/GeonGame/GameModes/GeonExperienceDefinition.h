// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GeonExperienceDefinition.generated.h"

class UGeonPawnData;

/**
 * 
 */
UCLASS()
class GEONGAME_API UGeonExperienceDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UGeonExperienceDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/*
	member variables
	*/
	UPROPERTY(EditDefaultsOnly, Category=Gameplay)
	TObjectPtr<UGeonPawnData> DefaultPawnData; 
	

	/**
	 * 해당 property는 단순히 마킹 및 기억용으로 남겨둔다
	 * - 게임 모드에 따른 GameFeature plugin을 로딩하는데 이에 대한 연결고리고 생각하면 된다
	 * - 후일 ShooterCore 관련 Plugin 작업할 경우, 진행하게 될 예정이다
	 */
	UPROPERTY(EditDefaultsOnly, Category=Gameplay)
	TArray<FString> GameFeaturesToEnable;
};
