// Fill out your copyright notice in the Description page of Project Settings.

#include "GeonGameModeBase.h"
#include "GeonGameState.h"
#include "GeonGame/GeonLogChannels.h"
#include "GeonGame/Character/GeonCharacter.h"
#include "GeonGame/Player/GeonPlayerController.h"
#include "GeonGame/Player/GeonPlayerState.h"
#include "GeonGame/GameModes/GeonExperienceManagerComponent.h"
#include "GeonExperienceManagerComponent.h"
#include "GeonGame/GameModes/GeonExperienceDefinition.h"
#include "GeonGame/Character/GeonPawnData.h"
#include "GeonGame/Character/GeonPawnExtensionComponent.h"

AGeonGameModeBase::AGeonGameModeBase()
{
	// �̷��� �صθ� ������ �� �״�� �Ѿ�� �ȴ�. 
	// Editor -> project setting -> GameMode�� ������ Ȯ�ΰ���
	GameStateClass = AGeonGameState::StaticClass();
	PlayerControllerClass = AGeonPlayerController::StaticClass();
	PlayerStateClass = AGeonPlayerState::StaticClass();
	DefaultPawnClass = AGeonCharacter::StaticClass();

}

void AGeonGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	// ���� GameInstance���� ���� �ʱ�ȭ �۾��� ����ǹǷ�, �� �����ӿ��� Lyra Concept�� Experience ó���� ������ �� ����.
	// �̸� ó���ϱ� ����, �� ������ �ڿ� �̺�Ʈ�� �޾� ó���� �̾ �����Ѵ�. 
	// ���ĵ��� ����..! 
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::HandleMatchAssignmentIfNotExpectingOne);
}

void AGeonGameModeBase::InitGameState()
{
	Super::InitGameState();

	// Experience �񵿱� �ε��� ���� Delegate�� �غ��Ѵ�.
	UGeonExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<UGeonExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	// OnExperienceLoaded ���
	ExperienceManagerComponent->CallOrRegister_OnExperienceLoaded(FOnGeonExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

UClass* AGeonGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	// GetPawnDataForController�� Ȱ���Ͽ�, PawnData�κ��� PawnClass�� ��������
	if (const UGeonPawnData* PawnData = GetPawnDataForController(InController))
	{
		if (PawnData->PawnClass)
		{
			return PawnData->PawnClass;
		}
	}

	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void AGeonGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	if (IsExperienceLoaded()) // �ε��� �ȵǾ� ������ ������ ���� �ʴ´�! 
	{
		Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	}
}

APawn* AGeonGameModeBase::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform)
{
	//UE_LOG(LogGeon, Log, TEXT("SpawnDefaultPawnAtTransform_Implementation is called!"));
	//return Super::SpawnDefaultPawnAtTransform_Implementation(NewPlayer, SpawnTransform);

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = GetInstigator();
	SpawnInfo.ObjectFlags |= RF_Transient;
	SpawnInfo.bDeferConstruction = true;

	if (UClass* PawnClass = GetDefaultPawnClassForController(NewPlayer))
	{
		if (APawn* SpawnedPawn = GetWorld()->SpawnActor<APawn>(PawnClass, SpawnTransform, SpawnInfo))
		{
			// FindPawnExtensionComponent ����
			if (UGeonPawnExtensionComponent* PawnExtComp = UGeonPawnExtensionComponent::FindPawnExtensionComponent(SpawnedPawn))
			{
				if (const UGeonPawnData* PawnData = GetPawnDataForController(NewPlayer))
				{
					PawnExtComp->SetPawnData(PawnData);
				}
			}

			SpawnedPawn->FinishSpawning(SpawnTransform);
			return SpawnedPawn;
		}
	}

	return nullptr;
}

void AGeonGameModeBase::HandleMatchAssignmentIfNotExpectingOne()
{
	// ������ Experience�� Experience Manager�� �����ش�. (Loading �ش޶��..!)
	// Lyra �� ���������� ��������⿡ �˻��� ���� ���� ..
	// �̷����� �˻��� �Ŀ� Experience�� Loading�ش޶�� ��û�Ѵ�	
	// �׷��� ������ �̸��� HandleMatchAssignment��... (�츮���� ũ�� ���� x) 

	// �ش� �Լ������� �츮�� �ε��� Experience�� ���� PrimaryAssetId�� �����Ͽ�, OnMatchAssignmentGiven���� �Ѱ��ش�

	FPrimaryAssetId ExperienceId;

	// precedence order (highest wins)
	// - matchmaking assignment (if present)
	// - default experience

	UWorld* World = GetWorld();

	// fall back to the default experience
	// �ϴ� �⺻ �ɼ����� default�ϰ� B_HakDefaultExperience�� ��������
	if (!ExperienceId.IsValid())
	{
		// ���� �ڵ�� B_GeonDefaultExperience�� �����ִµ� �ƹ������� B_LyraDefaultExperience�� �´µ�..?
		ExperienceId = FPrimaryAssetId(FPrimaryAssetType("GeonExperienceDefinition"), FName("B_GeonDefaultExperience"));
		//ExperienceId = FPrimaryAssetId(FPrimaryAssetType("GeonExperienceDefinition"), FName("B_LyraDefaultExperience"));
	}

	// ���ڰ� ������ HandleMatchAssignmentIfNotExpectingOne�� OnMatchAssignmentGiven()�� ���� ���������� �̸��� �ʹ��� �ʴ´ٰ� �����Ѵ�
	// - ����, ������� Lyra�� �����Ǹ�, �ش� �Լ��� ���� �� ������ �� ���� ������ �����Ѵ�
	OnMatchAssignmentGiven(ExperienceId);

}

void AGeonGameModeBase::OnMatchAssignmentGiven(FPrimaryAssetId ExperienceId)
{
	// �ش� �Լ��� ExperienceManagerComponent�� Ȱ���Ͽ� Experience�� �ε��ϱ� ����, ExperienceManagerComponent�� ServerSetCurrentExperience�� ȣ���Ѵ�

	check(ExperienceId.IsValid());

	UGeonExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<UGeonExperienceManagerComponent>();
	check(ExperienceManagerComponent);
	ExperienceManagerComponent->ServerSetCurrentExperience(ExperienceId);
}

bool AGeonGameModeBase::IsExperienceLoaded() const
{
	check(GameState);
	UGeonExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<UGeonExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	return ExperienceManagerComponent->IsExperienceLoaded();
}

void AGeonGameModeBase::OnExperienceLoaded(const UGeonExperienceDefinition* CurrentExperience)
{
	// PlayerController�� ��ȸ�ϸ�
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PC = Cast<APlayerController>(*Iterator);

		// PlayerController�� Pawn�� Possess���� �ʾҴٸ�, RestartPlayer�� ���� Pawn�� �ٽ� Spawn�Ѵ�
		// - �ѹ� OnPossess�� ������ ����:
		if (PC && PC->GetPawn() == nullptr)
		{
			if (PlayerCanRestart(PC))
			{
				RestartPlayer(PC);
			}
		}
	}
}

const UGeonPawnData* AGeonGameModeBase::GetPawnDataForController(const AController* InController) const
{
	// ���� ���߿� PawnData�� �������̵� �Ǿ��� ���, PawnData�� PlayerState���� �������� ��
	if (InController)
	{
		if (const AGeonPlayerState* GeonPS = InController->GetPlayerState<AGeonPlayerState>())
		{
			// GetPawnData ����
			if (const UGeonPawnData* PawnData = GeonPS->GetPawnData<UGeonPawnData>())
			{
				return PawnData;
			}
		}
	}

	// fall back to the default for the current experience
	// ���� PlayerState�� PawnData�� �����Ǿ� ���� ���� ���, ExperienceManagerComponent�� CurrentExperience�κ��� �����ͼ� ����
	check(GameState);
	UGeonExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<UGeonExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	if (ExperienceManagerComponent->IsExperienceLoaded())
	{
		// GetExperienceChecked ����
		const UGeonExperienceDefinition* Experience = ExperienceManagerComponent->GetCurrentExperienceChecked();
		if (Experience->DefaultPawnData)
		{
			return Experience->DefaultPawnData;
		}
	}

	// ��� ���̽����� �ڵ鸵 �ȵǾ����� nullptr
	return nullptr;
}
