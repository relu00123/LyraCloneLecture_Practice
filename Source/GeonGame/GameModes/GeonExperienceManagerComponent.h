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

public:
	UPROPERTY()
	TObjectPtr<const UGeonExperienceDefinition> CurrentExperience;

	// Experience�� �ε� ���¸� ����͸�
	EGeonExperienceLoadState LoadState = EGeonExperienceLoadState::Unloaded;

	// Experience �ε��� �Ϸ�� ����, Broadcasting Delegate
	FOnGeonExperienceLoaded OnExperienceLoaded;
};
