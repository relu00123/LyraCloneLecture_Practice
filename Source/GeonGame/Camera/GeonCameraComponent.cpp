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
	// Camera Component에서 매 프레임 호출된다 ? 
	check(CameraModeStack);

	UpdateCameraModes();

	// EvaluateStack은 CameraModeStack에 있는 CameraMode를 업데이트(+블랜딩)하고 CameraModeStack을 Bottom-Top까지 업데이트된 CameraMode들을 Lerp를 진행해준다.
	// - 이에 대한 결과는 CameraModeView에 캐싱된다
	FGeonCameraModeView CameraModeView;
	CameraModeStack->EvaluateStack(DeltaTime, CameraModeView);
}

void UGeonCameraComponent::UpdateCameraModes()
{
	check(CameraModeStack);

	// DetermineCameraModeDelegate는 CameraMode Class를 반환한다:
	// - 해당 delegate는 HeroComponent의 멤버 함수로 바인딩되어 있다
	if (DetermineCameraModeDelegate.IsBound())
	{
		if (TSubclassOf<UGeonCameraMode> CameraMode = DetermineCameraModeDelegate.Execute())
		{
			CameraModeStack->PushCameraMode(CameraMode);
		}
	}
}
