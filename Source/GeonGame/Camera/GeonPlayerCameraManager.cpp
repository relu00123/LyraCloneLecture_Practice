// Fill out your copyright notice in the Description page of Project Settings.


#include "GeonPlayerCameraManager.h"

AGeonPlayerCameraManager::AGeonPlayerCameraManager(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	DefaultFOV = HAK_CAMERA_DEFAULT_FOV;
	ViewPitchMin = HAK_CAMERA_DEFAULT_PITCH_MIN;
	ViewPitchMax = HAK_CAMERA_DEFAULT_PITCH_MAX;
}
