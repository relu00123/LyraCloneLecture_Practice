// Fill out your copyright notice in the Description page of Project Settings.


#include "GeonPlayerCameraManager.h"

AGeonPlayerCameraManager::AGeonPlayerCameraManager(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	DefaultFOV = GEON_CAMERA_DEFAULT_FOV;
	ViewPitchMin = GEON_CAMERA_DEFAULT_PITCH_MIN;
	ViewPitchMax = GEON_CAMERA_DEFAULT_PITCH_MAX;
}
