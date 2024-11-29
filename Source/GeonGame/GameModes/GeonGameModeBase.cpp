// Fill out your copyright notice in the Description page of Project Settings.

#include "GeonGameModeBase.h"
#include "GeonGameState.h"
#include "GeonGame/GeonLogChannels.h"
#include "GeonGame/Character/GeonCharacter.h"
#include "GeonGame/Player/GeonPlayerController.h"
#include "GeonGame/Player/GeonPlayerState.h"
#include "GeonGame/GameModes/GeonExperienceManagerComponent.h"

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

void AGeonGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	if (IsExperienceLoaded()) // 로딩이 안되어 있으면 수행을 하지 않는다! 
	{
		Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	}
}

APawn* AGeonGameModeBase::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform)
{
	UE_LOG(LogGeon, Log, TEXT("SpawnDefaultPawnAtTransform_Implementation is called!"));
	return Super::SpawnDefaultPawnAtTransform_Implementation(NewPlayer, SpawnTransform);
}

void AGeonGameModeBase::HandleMatchAssignmentIfNotExpectingOne()
{
	// 결정한 Experience를 Experience Manager에 던져준다. (Loading 해달라고..!)
	// Lyra 는 데디기반으로 만들어졌기에 검사할 것이 많다 ..
	// 이런것을 검사한 후에 Experience를 Loading해달라고 요청한다	
	// 그렇기 때문에 이름이 HandleMatchAssignment임... (우리랑은 크게 관련 x) 

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
}
