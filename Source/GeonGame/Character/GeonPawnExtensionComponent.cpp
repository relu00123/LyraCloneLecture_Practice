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
	IGameFrameworkInitStateInterface::OnActorInitStateChanged(Params);
}

bool UGeonPawnExtensionComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	return IGameFrameworkInitStateInterface::CanChangeInitState(Manager, CurrentState, DesiredState);
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

