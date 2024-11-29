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

void AGeonGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	if (IsExperienceLoaded()) // �ε��� �ȵǾ� ������ ������ ���� �ʴ´�! 
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
	// ������ Experience�� Experience Manager�� �����ش�. (Loading �ش޶��..!)
	// Lyra �� ���������� ��������⿡ �˻��� ���� ���� ..
	// �̷����� �˻��� �Ŀ� Experience�� Loading�ش޶�� ��û�Ѵ�	
	// �׷��� ������ �̸��� HandleMatchAssignment��... (�츮���� ũ�� ���� x) 

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
