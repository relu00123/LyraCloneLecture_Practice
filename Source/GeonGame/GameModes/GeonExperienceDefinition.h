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
	 * �ش� property�� �ܼ��� ��ŷ �� �������� ���ܵд�
	 * - ���� ��忡 ���� GameFeature plugin�� �ε��ϴµ� �̿� ���� ������� �����ϸ� �ȴ�
	 * - ���� ShooterCore ���� Plugin �۾��� ���, �����ϰ� �� �����̴�
	 */
	UPROPERTY(EditDefaultsOnly, Category=Gameplay)
	TArray<FString> GameFeaturesToEnable;
};
