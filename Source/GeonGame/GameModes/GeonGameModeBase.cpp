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
	// 이렇게 해두면 설정한 값 그대로 넘어가게 된다. 
	// Editor -> project setting -> GameMode에 가보면 확인가능
	GameStateClass = AGeonGameState::StaticClass();
	PlayerControllerClass = AGeonPlayerController::StaticClass();
	PlayerStateClass = AGeonPlayerState::StaticClass();
	DefaultPawnClass = AGeonCharacter::StaticClass();

}

void AGeonGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	// 아직 GameInstanceㄹㄹ 통해 초기화 작업이 진행되므로, 현 프레임에는 Lyra Concept인 Experience 처리를 진행할 수 없다.
	// 이를 처리하기 위해, 한 프레임 뒤에 이벤트를 받아 처리를 이어서 진행한다. 
	// 도식도를 봐라..! 
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::HandleMatchAssignmentIfNotExpectingOne);
}

void AGeonGameModeBase::InitGameState()
{
	Super::InitGameState();

	// Experience 비동기 로딩을 위한 Delegate를 준비한다.
	UGeonExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<UGeonExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	// OnExperienceLoaded 등록
	ExperienceManagerComponent->CallOrRegister_OnExperienceLoaded(FOnGeonExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

UClass* AGeonGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	// GetPawnDataForController를 활용하여, PawnData로부터 PawnClass를 유도하자
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
	if (IsExperienceLoaded()) // 로딩이 안되어 있으면 수행을 하지 않는다! 
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
			// FindPawnExtensionComponent 구현
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
	// 결정한 Experience를 Experience Manager에 던져준다. (Loading 해달라고..!)
	// Lyra 는 데디기반으로 만들어졌기에 검사할 것이 많다 ..
	// 이런것을 검사한 후에 Experience를 Loading해달라고 요청한다	
	// 그렇기 때문에 이름이 HandleMatchAssignment임... (우리랑은 크게 관련 x) 

	// 해당 함수에서는 우리가 로딩할 Experience에 대해 PrimaryAssetId를 생성하여, OnMatchAssignmentGiven으로 넘겨준다

	FPrimaryAssetId ExperienceId;

	// precedence order (highest wins)
	// - matchmaking assignment (if present)
	// - default experience

	UWorld* World = GetWorld();

	// fall back to the default experience
	// 일단 기본 옵션으로 default하게 B_HakDefaultExperience로 설정놓자
	if (!ExperienceId.IsValid())
	{
		// 강의 코드는 B_GeonDefaultExperience로 적여있는데 아무리봐도 B_LyraDefaultExperience가 맞는데..?
		ExperienceId = FPrimaryAssetId(FPrimaryAssetType("GeonExperienceDefinition"), FName("B_GeonDefaultExperience"));
		//ExperienceId = FPrimaryAssetId(FPrimaryAssetType("GeonExperienceDefinition"), FName("B_LyraDefaultExperience"));
	}

	// 필자가 이해한 HandleMatchAssignmentIfNotExpectingOne과 OnMatchAssignmentGiven()은 아직 직관적으로 이름이 와닫지 않는다고 생각한다
	// - 후일, 어느정도 Lyra가 구현되면, 해당 함수의 명을 더 이해할 수 있을 것으로 예상한다
	OnMatchAssignmentGiven(ExperienceId);

}

void AGeonGameModeBase::OnMatchAssignmentGiven(FPrimaryAssetId ExperienceId)
{
	// 해당 함수는 ExperienceManagerComponent을 활용하여 Experience을 로딩하기 위해, ExperienceManagerComponent의 ServerSetCurrentExperience를 호출한다

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
	// PlayerController를 순회하며
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PC = Cast<APlayerController>(*Iterator);

		// PlayerController가 Pawn을 Possess하지 않았다면, RestartPlayer를 통해 Pawn을 다시 Spawn한다
		// - 한번 OnPossess를 보도록 하자:
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
	// 게임 도중에 PawnData가 오버라이드 되었을 경우, PawnData는 PlayerState에서 가져오게 됨
	if (InController)
	{
		if (const AGeonPlayerState* GeonPS = InController->GetPlayerState<AGeonPlayerState>())
		{
			// GetPawnData 구현
			if (const UGeonPawnData* PawnData = GeonPS->GetPawnData<UGeonPawnData>())
			{
				return PawnData;
			}
		}
	}

	// fall back to the default for the current experience
	// 아직 PlayerState에 PawnData가 설정되어 있지 않은 경우, ExperienceManagerComponent의 CurrentExperience로부터 가져와서 설정
	check(GameState);
	UGeonExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<UGeonExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	if (ExperienceManagerComponent->IsExperienceLoaded())
	{
		// GetExperienceChecked 구현
		const UGeonExperienceDefinition* Experience = ExperienceManagerComponent->GetCurrentExperienceChecked();
		if (Experience->DefaultPawnData)
		{
			return Experience->DefaultPawnData;
		}
	}

	// 어떠한 케이스에도 핸들링 안되었으면 nullptr
	return nullptr;
}
