// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "GameFeaturePluginOperationResult.h"
#include "GeonExperienceManagerComponent.generated.h"

class UGeonExperienceDefinition;

enum class EGeonExperienceLoadState
{
	Unloaded,
	Loading,
	LoadingGameFeatures,
	ExecutingActions,
	Loaded,
	Deactivating,
};


DECLARE_MULTICAST_DELEGATE_OneParam(FOnGeonExperienceLoaded, const UGeonExperienceDefinition*);


/**
 GeonExperienceManagerComponent
 �� �״��, �ش� ������Ʈ�� game state�� owner�� �����鼭, experience�� ���� ������ ������ �ִ� component�̴�. 
 �Ӹ� �ƴ϶�, manager��� �ܾ ���ԵǾ� �ֵ���, experience �ε� ���� ������Ʈ �� �̺�Ʈ�� �����Ѵ�
 */
UCLASS()
class GEONGAME_API UGeonExperienceManagerComponent : public UGameStateComponent
{
	GENERATED_BODY()

public:
	bool IsExperienceLoaded() { return (LoadState == EGeonExperienceLoadState::Loaded) && (CurrentExperience != nullptr); }

	// �Ʒ��� OnExperienceLoaded�� ���ε��ϰų�, �̹� Experience �ε��� �Ϸ�Ǿ��ٸ� �ٷ� ȣ����
	void CallOrRegister_OnExperienceLoaded(FOnGeonExperienceLoaded::FDelegate&& Delegate);

	void ServerSetCurrentExperience(FPrimaryAssetId ExperienceId);
	void StartExperienceLoad();
	void OnExperienceLoadComplete();
	void OnGameFeaturePluginLoadComplete(const UE::GameFeatures::FResult& Result);
	void OnExperienceFullLoadCompleted();
	const UGeonExperienceDefinition* GetCurrentExperienceChecked() const; 



public:
	UPROPERTY()
	TObjectPtr<const UGeonExperienceDefinition> CurrentExperience;

	// Experience�� �ε� ���¸� ����͸�
	EGeonExperienceLoadState LoadState = EGeonExperienceLoadState::Unloaded;

	// Experience �ε��� �Ϸ�� ����, Broadcasting Delegate
	FOnGeonExperienceLoaded OnExperienceLoaded;


	// ��� GameFeature�� �ε��� �� ���ΰ�?
	int32 NumGameFeaturePluginsLoading = 0;
	TArray<FString> GameFeaturePluginURLs;


};


