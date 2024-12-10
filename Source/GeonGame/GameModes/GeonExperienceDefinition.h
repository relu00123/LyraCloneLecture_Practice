// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GeonExperienceDefinition.generated.h"

class UGeonPawnData;
class UGameFeatureAction;
class UGeonExperienceActionSet;

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


	/** ExperienceActionSet은 UGameFeatureAction의 Set이며, Gameplay 용도에 맞게 분류의 목적으로 사용한다 */
	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TArray<TObjectPtr<UGeonExperienceActionSet>> ActionSets;

	/** 일반적인 GameFeatureAction으로서 추가 */
	UPROPERTY(EditDefaultsOnly, Instanced, Category = "Actions")
	TArray<TObjectPtr<UGameFeatureAction>> Actions;

};
