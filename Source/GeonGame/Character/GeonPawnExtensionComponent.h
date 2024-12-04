// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "GeonPawnExtensionComponent.generated.h"


class UGeonPawnData;
/**
 * 
 */
UCLASS()
class GEONGAME_API UGeonPawnExtensionComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()

public: 
	UGeonPawnExtensionComponent(const FObjectInitializer& ObjectInitialize = FObjectInitializer::Get());

	// FeatureName 정의
	static const FName NAME_ActorFeatureName;

	
	//member methods
	static UGeonPawnExtensionComponent* FindPawnExtensionComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UGeonPawnExtensionComponent>() : nullptr); }
	template <class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }
	void SetPawnData(const UGeonPawnData* InPawnData);
	void SetupPlayerInputComponent();


	// UPawnComponent interfaces
	virtual void OnRegister() final;
	virtual void BeginPlay() final;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) final;

	virtual FName GetFeatureName() const final { return NAME_ActorFeatureName; }
	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) final;
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const final;
	virtual void CheckDefaultInitialization() final;

	// Pawn을 생성한 데이터를 캐싱
	UPROPERTY(EditInstanceOnly, Category = "Geon|Pawn")
	TObjectPtr<const UGeonPawnData> PawnData;

};
