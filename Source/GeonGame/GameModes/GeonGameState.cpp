// Fill out your copyright notice in the Description page of Project Settings.


#include "GeonGameState.h"
#include "GeonGame/GameModes/GeonExperienceManagerComponent.h"

AGeonGameState::AGeonGameState()
{
	ExperienceManagerComponent = CreateDefaultSubobject<UGeonExperienceManagerComponent>(TEXT("ExperienceManagerComponent"));
}
