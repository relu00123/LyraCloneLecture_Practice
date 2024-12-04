// Fill out your copyright notice in the Description page of Project Settings.


#include "GeonPlayerController.h"
#include "GeonGame/Camera/GeonPlayerCameraManager.h"

 

AGeonPlayerController::AGeonPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PlayerCameraManagerClass = AGeonPlayerCameraManager::StaticClass();
}