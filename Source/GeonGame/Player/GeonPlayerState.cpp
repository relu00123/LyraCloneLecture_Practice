// Fill out your copyright notice in the Description page of Project Settings.


#include "GeonPlayerState.h"
#include "GeonGame/GameModes/GeonExperienceManagerComponent.h"

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
}
