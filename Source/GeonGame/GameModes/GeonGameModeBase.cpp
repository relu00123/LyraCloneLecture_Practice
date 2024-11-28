// Fill out your copyright notice in the Description page of Project Settings.

#include "GeonGameModeBase.h"
#include "GeonGameState.h"
#include "GeonGame/Character/GeonCharacter.h"
#include "GeonGame/Player/GeonPlayerController.h"
#include "GeonGame/Player/GeonPlayerState.h"

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

void AGeonGameModeBase::HandleMatchAssignmentIfNotExpectingOne()
{
	// ������ Experience�� Experience Manager�� �����ش�. (Loading �ش޶��..!)
	// Lyra �� ���������� ��������⿡ �˻��� ���� ���� ..
	// �̷����� �˻��� �Ŀ� Experience�� Loading�ش޶�� ��û�Ѵ�
	// �׷��� ������ �̸��� HandleMatchAssignment��... (�츮���� ũ�� ���� x) 

}
