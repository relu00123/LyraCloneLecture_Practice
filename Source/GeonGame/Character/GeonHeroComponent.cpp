// Fill out your copyright notice in the Description page of Project Settings.


#include "GeonHeroComponent.h"
#include "GeonPawnExtensionComponent.h"
#include "GeonGame/GeonLogChannels.h"
#include "GeonGame/GeonGameplayTags.h"
#include "GeonGame/Player/GeonPlayerState.h"
#include "Components/GameFrameworkComponentManager.h"
#include "GeonGame/Character/GeonPawnData.h"

/** FeatureName 정의: static member variable 초기화 */
const FName UGeonHeroComponent::NAME_ActorFeatureName("Hero");

UGeonHeroComponent::UGeonHeroComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
}

void UGeonHeroComponent::OnRegister()
{
	Super::OnRegister();

	// 올바른 Actor에 등록되었는지 확인:
	{
		if (!GetPawn<APawn>())
		{
			UE_LOG(LogGeon, Error, TEXT("this component has been added to a BP whose base class is not a Pawn!"));
			return;
		}
	}

	RegisterInitStateFeature();
}

void UGeonHeroComponent::BeginPlay()
{
	Super::BeginPlay();

	// PawnExtensionComponent에 대해서 (PawnExtension Feature) OnActorInitStateChanged() 관찰하도록 (Observing)
	BindOnActorInitStateChanged(UGeonPawnExtensionComponent::NAME_ActorFeatureName, FGameplayTag(), false);

	// InitState_Spawned로 초기화
	ensure(TryToChangeInitState(FGeonGameplayTags::Get().InitState_Spawned));

	// ForceUpdate 진행
	CheckDefaultInitialization();
}

void UGeonHeroComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterInitStateFeature();

	Super::EndPlay(EndPlayReason);
}

void UGeonHeroComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	const FGeonGameplayTags& InitTags = FGeonGameplayTags::Get();

	if (Params.FeatureName == UGeonPawnExtensionComponent::NAME_ActorFeatureName)
	{
		// HakPawnExtensionComponent의 DataInitialized 상태 변화 관찰 후, HakHeroComponent도 DataInitialized 상태로 변경
		// - CanChangeInitState 확인
		if (Params.FeatureState == InitTags.InitState_DataInitialized)
		{
			CheckDefaultInitialization();
		}
	}
}

bool UGeonHeroComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	check(Manager);

	const FGeonGameplayTags& InitTags = FGeonGameplayTags::Get();
	APawn* Pawn = GetPawn<APawn>();
	AGeonPlayerState* GeonPS = GetPlayerState<AGeonPlayerState>();

	// Spawned 초기화
	if (!CurrentState.IsValid() && DesiredState == InitTags.InitState_Spawned)
	{
		if (Pawn)
		{
			return true;
		}
	}

	// Spawned -> DataAvailable
	if (CurrentState == InitTags.InitState_Spawned && DesiredState == InitTags.InitState_DataAvailable)
	{
		if (!GeonPS)
		{
			return false;
		}

		return true;
	}

	// DataAvailable -> DataInitialized
	if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		// PawnExtensionComponent가 DataInitialized될 때까지 기다림 (== 모든 Feature Component가 DataAvailable인 상태)
		return GeonPS && Manager->HasFeatureReachedInitState(Pawn, UGeonPawnExtensionComponent::NAME_ActorFeatureName, InitTags.InitState_DataInitialized);
	}

	// DataInitialized -> GameplayReady
	if (CurrentState == InitTags.InitState_DataInitialized && DesiredState == InitTags.InitState_GameplayReady)
	{
		return true;
	}

	return false;
}

void UGeonHeroComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState)
{
	const FGeonGameplayTags& InitTags = FGeonGameplayTags::Get();

	// DataAvailable -> DataInitialized 단계
	if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		APawn* Pawn = GetPawn<APawn>();
		AGeonPlayerState* GeonPS = GetPlayerState<AGeonPlayerState>();
		if (!ensure(Pawn && GeonPS))
		{
			return;
		}

		// Input과 Camera에 대한 핸들링... (TODO)

		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
		const UGeonPawnData* PawnData = nullptr;
		if (UGeonPawnExtensionComponent* PawnExtComp = UGeonPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			PawnData = PawnExtComp->GetPawnData<UGeonPawnData>();
		}

		// Camera에 대한 Handling을 해볼 것임.. 	 
	}
}

void UGeonHeroComponent::CheckDefaultInitialization()
{
	// 앞서 BindOnActorInitStateChanged에서 보았듯이 Hero Feature는 Pawn Extension Feature에 종속되어 있으므로, CheckDefaultInitializationForImplementers 호출하지 않음:

	// ContinueInitStateChain은 앞서 PawnExtComponent와 같음
	const FGeonGameplayTags& InitTags = FGeonGameplayTags::Get();
	static const TArray<FGameplayTag> StateChain = { InitTags.InitState_Spawned, InitTags.InitState_DataAvailable, InitTags.InitState_DataInitialized, InitTags.InitState_GameplayReady };
	ContinueInitStateChain(StateChain);
}
