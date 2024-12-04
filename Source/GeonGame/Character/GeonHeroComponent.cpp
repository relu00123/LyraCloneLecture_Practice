// Fill out your copyright notice in the Description page of Project Settings.


#include "GeonHeroComponent.h"
#include "GeonPawnExtensionComponent.h"
#include "GeonGame/GeonLogChannels.h"
#include "GeonGame/GeonGameplayTags.h"
#include "GeonGame/Player/GeonPlayerState.h"
#include "Components/GameFrameworkComponentManager.h"
#include "GeonGame/Character/GeonPawnData.h"
#include "GeonGame/Camera/GeonCameraComponent.h"

/** FeatureName ����: static member variable �ʱ�ȭ */
const FName UGeonHeroComponent::NAME_ActorFeatureName("Hero");

UGeonHeroComponent::UGeonHeroComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
}

void UGeonHeroComponent::OnRegister()
{
	Super::OnRegister();

	// �ùٸ� Actor�� ��ϵǾ����� Ȯ��:
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

	// PawnExtensionComponent�� ���ؼ� (PawnExtension Feature) OnActorInitStateChanged() �����ϵ��� (Observing)
	BindOnActorInitStateChanged(UGeonPawnExtensionComponent::NAME_ActorFeatureName, FGameplayTag(), false);

	// InitState_Spawned�� �ʱ�ȭ
	ensure(TryToChangeInitState(FGeonGameplayTags::Get().InitState_Spawned));

	// ForceUpdate ����
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
		// GeonPawnExtensionComponent�� DataInitialized ���� ��ȭ ���� ��, GeonHeroComponent�� DataInitialized ���·� ����
		// - CanChangeInitState Ȯ��
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

	// Spawned �ʱ�ȭ
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
		// PawnExtensionComponent�� DataInitialized�� ������ ��ٸ� (== ��� Feature Component�� DataAvailable�� ����)
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

	// DataAvailable -> DataInitialized �ܰ�
	if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		APawn* Pawn = GetPawn<APawn>();
		AGeonPlayerState* GeonPS = GetPlayerState<AGeonPlayerState>();
		if (!ensure(Pawn && GeonPS))
		{
			return;
		}

		// Input�� Camera�� ���� �ڵ鸵... (TODO)

		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
		const UGeonPawnData* PawnData = nullptr;
		if (UGeonPawnExtensionComponent* PawnExtComp = UGeonPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			PawnData = PawnExtComp->GetPawnData<UGeonPawnData>();
		}

		// Camera�� ���� Handling
		if (bIsLocallyControlled && PawnData)
		{
			// ���� GeonCharacter�� Attach�� CameraComponent�� ã��
			if (UGeonCameraComponent* CameraComponent = UGeonCameraComponent::FindCameraComponent(Pawn))
			{
				CameraComponent->DetermineCameraModeDelegate.BindUObject(this, &ThisClass::DetermineCameraMode);
			}
		}
	}
}

void UGeonHeroComponent::CheckDefaultInitialization()
{
	// �ռ� BindOnActorInitStateChanged���� ���ҵ��� Hero Feature�� Pawn Extension Feature�� ���ӵǾ� �����Ƿ�, CheckDefaultInitializationForImplementers ȣ������ ����:

	// ContinueInitStateChain�� �ռ� PawnExtComponent�� ����
	const FGeonGameplayTags& InitTags = FGeonGameplayTags::Get();
	static const TArray<FGameplayTag> StateChain = { InitTags.InitState_Spawned, InitTags.InitState_DataAvailable, InitTags.InitState_DataInitialized, InitTags.InitState_GameplayReady };
	ContinueInitStateChain(StateChain);
}

PRAGMA_DISABLE_OPTIMIZATION
TSubclassOf<UGeonCameraMode> UGeonHeroComponent::DetermineCameraMode() const
{
	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return nullptr;
	}

	if (UGeonPawnExtensionComponent* PawnExtComp = UGeonPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (const UGeonPawnData* PawnData = PawnExtComp->GetPawnData<UGeonPawnData>())
		{
			return PawnData->DefaultCameraMode;
		}
	}

	return nullptr;
}
PRAGMA_ENABLE_OPTIMIZATION

