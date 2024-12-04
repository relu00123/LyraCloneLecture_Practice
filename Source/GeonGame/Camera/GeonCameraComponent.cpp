// Fill out your copyright notice in the Description page of Project Settings.


#include "GeonCameraComponent.h"
#include "GeonCameraMode.h"

UGeonCameraComponent::UGeonCameraComponent(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer), CameraModeStack(nullptr)
{
}

void UGeonCameraComponent::OnRegister()
{
	Super::OnRegister();

	if (!CameraModeStack) {
		CameraModeStack = NewObject<UGeonCameraModeStack>(this);
	}
}

void UGeonCameraComponent::GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView)
{
	// Camera Component���� �� ������ ȣ��ȴ� ? 
	check(CameraModeStack);

	UpdateCameraModes();
}

void UGeonCameraComponent::UpdateCameraModes()
{
	check(CameraModeStack);

	// DetermineCameraModeDelegate�� CameraMode Class�� ��ȯ�Ѵ�:
	// - �ش� delegate�� HeroComponent�� ��� �Լ��� ���ε��Ǿ� �ִ�
	if (DetermineCameraModeDelegate.IsBound())
	{
		if (TSubclassOf<UGeonCameraMode> CameraMode = DetermineCameraModeDelegate.Execute())
		{
			//CameraModeStack->PushCameraMode(CameraMode);
		}
	}
}
