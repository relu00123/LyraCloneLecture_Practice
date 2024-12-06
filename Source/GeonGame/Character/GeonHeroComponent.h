// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "Components/PawnComponent.h"
#include "GeonGame/Input/GeonMappableConfigPair.h"
#include "GeonHeroComponent.generated.h"


struct FGeonMappableConfigPair;
class UGeonCameraMode;
 

/**
  ī�޶�, �Է� �� �÷��̾ �����ϴ� �ý����� �ʱ�ȭ�� ó���ϴ� ������Ʈ 
 */
UCLASS(Blueprintable, Meta = (BlueprintSpawnableComponent))
class GEONGAME_API UGeonHeroComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()

public:
	UGeonHeroComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Feature Name ���� 
	static const FName NAME_ActorFeatureName;

	// UPawnComponent interface
	virtual void OnRegister() final;
	virtual void BeginPlay() final;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) final;

	// IGameFrameworkInitStateInterface
	virtual FName GetFeatureName() const final { return NAME_ActorFeatureName; }
	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) final;
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const final;
	virtual void HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) final;
	virtual void CheckDefaultInitialization() final;
	
	// Member methods
	TSubclassOf<UGeonCameraMode> DetermineCameraMode() const;

	// member variables 
	UPROPERTY(EditAnywhere)
	TArray<FGeonMappableConfigPair> DefaultInputConfigs;

};
