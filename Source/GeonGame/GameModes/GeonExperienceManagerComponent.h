// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "GeonExperienceManagerComponent.generated.h"

class UGeonExperienceDefinition;

enum class EGeonExperienceLoadState
{
	Unloaded,
	Loading,
	Loaded,
	Deactivating,
};


DECLARE_MULTICAST_DELEGATE_OneParam(FOnGeonExperienceLoaded, const UGeonExperienceDefinition*);


/**
 GeonExperienceManagerComponent
 말 그대로, 해당 컴포넌트는 game state를 owner로 가지면서, experience의 상태 정보를 가지고 있는 component이다. 
 뿐만 아니라, manager라는 단어가 포함되어 있듯이, experience 로딩 상태 업데이트 및 이벤트를 관리한다
 */
UCLASS()
class GEONGAME_API UGeonExperienceManagerComponent : public UGameStateComponent
{
	GENERATED_BODY()

public:
	bool IsExperienceLoaded() { return (LoadState == EGeonExperienceLoadState::Loaded) && (CurrentExperience != nullptr); }

	// 아래의 OnExperienceLoaded에 바인딩하거나, 이미 Experience 로딩이 완료되었다면 바로 호출함
	void CallOrRegister_OnExperienceLoaded(FOnGeonExperienceLoaded::FDelegate&& Delegate);

public:
	UPROPERTY()
	TObjectPtr<const UGeonExperienceDefinition> CurrentExperience;

	// Experience의 로딩 상태를 모니터링
	EGeonExperienceLoadState LoadState = EGeonExperienceLoadState::Unloaded;

	// Experience 로딩이 완료된 이후, Broadcasting Delegate
	FOnGeonExperienceLoaded OnExperienceLoaded;
};
