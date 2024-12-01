// Fill out your copyright notice in the Description page of Project Settings.


#include "GeonPlayerState.h"
#include "GeonGame/GameModes/GeonExperienceManagerComponent.h"
#include "GeonGame/GameModes/GeonGameModeBase.h"
#include "GeonGame/Character/GeonPawnData.h"

void AGeonPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AGameStateBase* GameState = GetWorld()->GetGameState();
	check(GameState);

	UGeonExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<UGeonExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	ExperienceManagerComponent->CallOrRegister_OnExperienceLoaded(FOnGeonExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

void AGeonPlayerState::OnExperienceLoaded(const UGeonExperienceDefinition* CurrentExperience)
{
	if (AGeonGameModeBase* GameMode = GetWorld()->GetAuthGameMode<AGeonGameModeBase>())
	{
		// AHakGameMode���� GetPawnDataForController�� �����ؾ� ��
		// - GetPawnDataForController���� �츮�� ���� PawnData�� �������� �ʾ����Ƿ�, ExperienceMangerComponent�� DefaultPawnData�� �����Ѵ�
		const UGeonPawnData* NewPawnData = GameMode->GetPawnDataForController(GetOwningController());
		check(NewPawnData);

		SetPawnData(NewPawnData);
	}
}

void AGeonPlayerState::SetPawnData(const UGeonPawnData* InPawnData)
{
	check(InPawnData);

	// PawnData�� �ι� �����Ǵ� ���� ������ ����!
	check(!PawnData);

	PawnData = InPawnData;
}
