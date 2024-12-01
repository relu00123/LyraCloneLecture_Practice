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
		// AHakGameMode에서 GetPawnDataForController를 구현해야 함
		// - GetPawnDataForController에서 우리는 아직 PawnData를 설정하지 않았으므로, ExperienceMangerComponent의 DefaultPawnData로 설정한다
		const UGeonPawnData* NewPawnData = GameMode->GetPawnDataForController(GetOwningController());
		check(NewPawnData);

		SetPawnData(NewPawnData);
	}
}

void AGeonPlayerState::SetPawnData(const UGeonPawnData* InPawnData)
{
	check(InPawnData);

	// PawnData가 두번 설정되는 것은 원하지 않음!
	check(!PawnData);

	PawnData = InPawnData;
}
