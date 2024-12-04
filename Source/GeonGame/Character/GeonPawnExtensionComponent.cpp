// Fill out your copyright notice in the Description page of Project Settings.


#include "GeonPawnExtensionComponent.h"

#include "Components/GameFrameworkComponentManager.h"
#include "GeonGame/GeonGameplayTags.h"
#include "GeonGame/GeonLogChannels.h"

// feature name�� Component�����ϱ� Component�� ���� pawn Extension�� ���� ���� Ȯ���� �� �ִ�.
const FName UGeonPawnExtensionComponent::NAME_ActorFeatureName("PawnExtension");

UGeonPawnExtensionComponent::UGeonPawnExtensionComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
}

void UGeonPawnExtensionComponent::SetPawnData(const UGeonPawnData* InPawnData)
{
	// Pawn�� ���� Authority�� ���� ���, SetPawnData�� �������� ����
	APawn* Pawn = GetPawnChecked<APawn>();
	if (Pawn->GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	if (PawnData)
	{
		return;
	}

	// PawnData ������Ʈ
	PawnData = InPawnData;
}

void UGeonPawnExtensionComponent::SetupPlayerInputComponent()
{
	// ForceUpdate�� �ٽ� InitState ���� ��ȯ ���� (���� ��)
	CheckDefaultInitialization();
}

void UGeonPawnExtensionComponent::OnRegister()
{
	Super::OnRegister();

	// �ùٸ� Actor�� ��ϵǾ����� Ȯ�� : 
	{
		if (!GetPawn<APawn>())
		{
			UE_LOG(LogGeon, Error, TEXT("this component has been added to a BP whose base class is not a Pawn!"));
			return; 
		}
	}

	// GameFrameworkComponentManager�� InitState ����� ���� ��� ����:
	// - ����� ��ӹ޾Ҵ� IGameFrameworkInitStateInterface �޼��� RegisterInitStateFeature()�� Ȱ��
	// - �ش� �Լ��� ������ ����
	RegisterInitStateFeature();

	// ������� ���� �Լ�
	UGameFrameworkComponentManager* Manager = UGameFrameworkComponentManager::GetForActor(GetOwningActor());
}

void UGeonPawnExtensionComponent::BeginPlay()
{
	Super::BeginPlay();

	// FeatureName�� NAME_None�� ������, Actor�� ��ϵ� Feature Component�� InitState ���¸� �����ϰڴٴ� �ǹ�:
	BindOnActorInitStateChanged(NAME_None, FGameplayTag(), false);

	// InitState_Spawned�� ���� ��ȯ:
	// - TryToChangeInitState�� �Ʒ��� ���� ����ȴ�:
	//   1. CanChangeInitState�� ���� ��ȯ ���ɼ� ���� �Ǵ�
	//   2. HandleChangeInitState�� ���� ���� ���� (Feature Component)
	//   3. BindOnActorInitStateChanged�� Bind�� Delegate�� ���ǿ� �°� ȣ���� ��
	//      - HakPawnExtensionComponent�� ���, ��� Actor�� Feature ���� ��ȭ�� ���� OnActorInitStateChanged()�� ȣ���
	ensure(TryToChangeInitState(FGeonGameplayTags::Get().InitState_Spawned));

	// �ش� �Լ��� �츮�� �������̵� �Ѵ�:
	// - �� �Լ��� ForceUpdateInitState�� ���� �������� �������ָ� �ȴ� 
	// - ���� ���� ������Ʈ ���� (���� CanChangeInitState�� HandleChangleInitState�� �������ش�)
	CheckDefaultInitialization();
}

void UGeonPawnExtensionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// �ռ�, OnRegister�� RegisterInitStateFeature()�� ���� ���߾�����
	UnregisterInitStateFeature();

	Super::EndPlay(EndPlayReason);
}

void UGeonPawnExtensionComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	if (Params.FeatureName != NAME_ActorFeatureName)
	{
		// HakPawnExtensionComponent�� �ٸ� Feature Component���� ���°� DataAvailable�� �����Ͽ�, Sync�� ���ߴ� ������ �־��� (CanChangeInitState)
		// - �̸� �������ϱ� ����, OnActorInitStateChanged������ DataAvailable�� ���� ���������� CheckDefaultInitialization�� ȣ���Ͽ�, ���¸� Ȯ���Ѵ�
		const FGeonGameplayTags& InitTags = FGeonGameplayTags::Get();
		if (Params.FeatureState == InitTags.InitState_DataAvailable)
		{
			CheckDefaultInitialization();
		}
	}
}

bool UGeonPawnExtensionComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	check(Manager);

	APawn* Pawn = GetPawn<APawn>();
	const FGeonGameplayTags& InitTags = FGeonGameplayTags::Get();

	// InitState_Spawned �ʱ�ȭ
	if (!CurrentState.IsValid() && DesiredState == InitTags.InitState_Spawned)
	{
		// Pawn�� �� ���ø� �Ǿ������� �ٷ� Spawned�� �Ѿ!
		if (Pawn)
		{
			return true;
		}
	}

	// Spawned -> DataAvailable
	if (CurrentState == InitTags.InitState_Spawned && DesiredState == InitTags.InitState_DataAvailable)
	{
		// �Ƹ� PawnData�� ������ �����ϴ� ����̴�
		if (!PawnData)
		{
			return false;
		}

		// LocallyControlled�� Pawn�� Controller�� ������ ����!
		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
		if (bIsLocallyControlled)
		{
			if (!GetController<AController>())
			{
				return false;
			}
		}

		return true;
	}

	// DataAvailable -> DataInitialized
	if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		// Actor�� ���ε�� ��� Feature���� DataAvailable ������ ��, DataInitialized�� �Ѿ:
		// - HaveAllFeaturesReachedInitState Ȯ��
		return Manager->HaveAllFeaturesReachedInitState(Pawn, InitTags.InitState_DataAvailable);
	}

	// DataInitialized -> GameplayReady
	if (CurrentState == InitTags.InitState_DataInitialized && DesiredState == InitTags.InitState_GameplayReady)
	{
		return true;
	}

	// ���� ��������(linear) transition�� �ƴϸ� false!
	return false;
}

void UGeonPawnExtensionComponent::CheckDefaultInitialization()
{
	// PawnExtensionComponent�� Feature Component���� �ʱ�ȭ�� �����ϴ� Component�̴�:
	// - ����, Actor�� ���ε��� Feature Component�鿡 ���� CheckDefaultInitialization�� ȣ�����ֵ��� �Ѵ� (ForceUpdate ����?)
	// - �� �޼��带 IGameFrameworkInitStateInterface�� �����ϴµ�, CheckDefaultInitializationForImplementers�̴�:
	// - ������ CheckDefaultInitializationForImplementers ����:
	CheckDefaultInitializationForImplementers();

	const FGeonGameplayTags& InitTags = FGeonGameplayTags::Get();

	// ����� ���� InitState�� ���� �Ѱ���� �Ѵ�... (��� �̰� �� �˾Ƽ� �� �� ������ ������ ����...)
	static const TArray<FGameplayTag> StateChain = { InitTags.InitState_Spawned, InitTags.InitState_DataAvailable, InitTags.InitState_DataInitialized, InitTags.InitState_GameplayReady };

	// CanChangeInitState()�� HandleChangeInitState() �׸��� ChangeFeatureInitState ȣ���� ���� OnActorInitStateChanged Delegate ȣ����� �������ش�:
	// �Ʒ��� �ڵ带 ������ ����:
	// - �ش� �ڵ���� ����, �츮�� ������ ������ �� �ִ�:
	//   - ��� ���¸� �����Ҷ����� �ѹ� ������Ʈ �Ѵ�
	//   - InitState�� ���� ��ȭ�� Linear(������)����!... -> ���߿� Diagram���� �����ϸ鼭 ����
	//     - ������Ʈ�� ���߸� ������ ��������� ���� �ǹ�! (chaining)
	ContinueInitStateChain(StateChain);
}

