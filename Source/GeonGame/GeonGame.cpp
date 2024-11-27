// Copyright Epic Games, Inc. All Rights Reserved.

#include "GeonGame.h"
#include "GeonLogChannels.h"
#include "Modules/ModuleManager.h"

class FGeonGameModule : public FDefaultGameModuleImpl
{
public : 
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
 
void FGeonGameModule::StartupModule()
{
	FDefaultGameModuleImpl::StartupModule();

	UE_LOG(LogTemp, Warning, L"Default Log!");

	UE_LOG(LogGeon, Warning, L"GeonGameModule");
}

void FGeonGameModule::ShutdownModule()
{
}

IMPLEMENT_PRIMARY_GAME_MODULE(FGeonGameModule, GeonGame, "GeonGame");

