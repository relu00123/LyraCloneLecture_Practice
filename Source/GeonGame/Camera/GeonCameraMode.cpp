// Fill out your copyright notice in the Description page of Project Settings.


#include "GeonCameraMode.h"
#include "GeonPlayerCameraManager.h"
#include "GeonCameraComponent.h"

FGeonCameraModeView::FGeonCameraModeView()
	: Location(ForceInit)
	, Rotation(ForceInit)
	, ControlRotation(ForceInit)
	, FieldOfView(GEON_CAMERA_DEFAULT_FOV)
{ 
}


UGeonCameraMode::UGeonCameraMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	FieldOfView = GEON_CAMERA_DEFAULT_FOV;
	ViewPitchMin = GEON_CAMERA_DEFAULT_PITCH_MIN;
	ViewPitchMax = GEON_CAMERA_DEFAULT_PITCH_MAX;

	BlendTime = 0.0f;
	BlendAlpha = 1.0f;
	BlendWeight = 1.0f;

	BlendFunction = EGeonCameraModeBlendFunction::EaseOut;
	BlendExponent = 4.0f;
}

void UGeonCameraMode::UpdateCameraMode(float DeltaTime)
{
	// Actor를 활용하여, Pivot[Location|Rotation]을 계산하여, View를 업데이트
	UpdateView(DeltaTime);

	// BlendWeight를 DeltaTime을 활용하여, BlendAlpha 계산 후, BlendFunction에 맞게 재-매핑하여 최종 계산
	UpdateBlending(DeltaTime);
}

void UGeonCameraMode::UpdateView(float DeltaTime)
{
	// CameraMode를 가지고 있는 CameraComponent의 Owner인 Character(Pawn)을 활용하여, PivotLocation/Rotation을 반환함
	FVector PivotLocation = GetPivotLocation();
	FRotator PivotRotation = GetPivotRotation();

	// Pitch 값에 대해 Min/Max를 Clamp시킴
	PivotRotation.Pitch = FMath::ClampAngle(PivotRotation.Pitch, ViewPitchMin, ViewPitchMax);

	// FGeonCameraModeView에 PivotLocation/Rotation 설정
	View.Location = PivotLocation;
	View.Rotation = PivotRotation;

	// PivotRotation을 똑같이 ControlRotation으로 활용
	View.ControlRotation = View.Rotation;
	View.FieldOfView = FieldOfView;

	// 정리하면, Character의 Location과 ControlRotation을 활용하여, View를 업데이트함
}

void UGeonCameraMode::UpdateBlending(float DeltaTime)
{
	// BlendAlpha를 DeltaTime을 통해 계산
	if (BlendTime > 0.f)
	{
		// BlendTime은 Blending 과정 총 시간(초)
		// - BlendAlpha는 0 -> 1로 변화하는 과정:
		// - DeltaTime을 활용하여, BlendTime을 1로 볼 경우, 진행 정도를 누적
		BlendAlpha += (DeltaTime / BlendTime);
	}
	else
	{
		BlendAlpha = 1.0f;
	}

	// BlendWeight를 [0,1]를 BlendFunction에 맞게 재매핑
	const float Exponent = (BlendExponent > 0.0f) ? BlendExponent : 1.0f;
	switch (BlendFunction)
	{
	case EGeonCameraModeBlendFunction::Linear:
		BlendWeight = BlendAlpha;
		break;
	case EGeonCameraModeBlendFunction::EaseIn:
		BlendWeight = FMath::InterpEaseIn(0.0f, 1.0f, BlendAlpha, Exponent);
		break;
	case EGeonCameraModeBlendFunction::EaseOut:
		BlendWeight = FMath::InterpEaseOut(0.0f, 1.0f, BlendAlpha, Exponent);
		break;
	case EGeonCameraModeBlendFunction::EaseInOut:
		BlendWeight = FMath::InterpEaseInOut(0.0f, 1.0f, BlendAlpha, Exponent);
		break;
	default:
		checkf(false, TEXT("UpdateBlending: Invalid BlendFunction [%d]\n"), (uint8)BlendFunction);
		break;
	}
}

UGeonCameraComponent* UGeonCameraMode::GetGeonCameraComponent() const
{
	// 우리가 앞서 UHakCameraMode를 생성하는 곳은 UHakCameraModeStack이었다:
	// - 해당 코드를 보면, GetOuter()를 HakCameraMode로 HakCameraComponent로 설정하였다
	// - UHakCameraModeStack::GetCameraModeInstance() 확인
	return CastChecked<UGeonCameraComponent>(GetOuter());
}

AActor* UGeonCameraMode::GetTargetActor() const
{
	const UGeonCameraComponent* GeonCameraComponent = GetGeonCameraComponent();
	return GeonCameraComponent->GetTargetActor();
}

FVector UGeonCameraMode::GetPivotLocation() const
{
	const AActor* TargetActor = GetTargetActor();
	check(TargetActor);

	if (const APawn* TargetPawn = Cast<APawn>(TargetActor))
	{
		// BaseEyeHeight를 고려하여, ViewLocation을 반환함
		return TargetPawn->GetPawnViewLocation();
	}

	return TargetActor->GetActorLocation();
}

FRotator UGeonCameraMode::GetPivotRotation() const
{
	const AActor* TargetActor = GetTargetActor();
	check(TargetActor);
 
	if (const APawn* TargetPawn = Cast<APawn>(TargetActor))
	{
		// GetViewRoation() 확인, 보통 Pawn의 ControlRotation을 반환
		return TargetPawn->GetViewRotation();
	}

	return TargetActor->GetActorRotation();
}

UGeonCameraModeStack::UGeonCameraModeStack(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UGeonCameraMode* UGeonCameraModeStack::GetCameraModeInstance(TSubclassOf<UGeonCameraMode>& CameraModeClass)
{
	check(CameraModeClass);

	// CameraModeInstances에서 먼저 생성되어있는지 확인 후, 반환한다:
	for (UGeonCameraMode* CameraMode : CameraModeInstances)
	{
		// CameraMode는 UClass를 비교한다:
		// - 즉, CameraMode는 클래스 타입에 하나만 생기게된다
		if ((CameraMode != nullptr) && (CameraMode->GetClass() == CameraModeClass))
		{
			return CameraMode;
		}
	}

	// CameraModeClass에 알맞는 CameraMode의 인스턴스가 없다면 생성한다:
	UGeonCameraMode* NewCameraMode = NewObject<UGeonCameraMode>(GetOuter(), CameraModeClass, NAME_None, RF_NoFlags);
	check(NewCameraMode);

	// 여기서 우리는 CameraModeInstances는 CameraModeClass에 맞는 인스턴스를 가지고(관리하는) 컨테이너이라는 것을 알 수 있다
	CameraModeInstances.Add(NewCameraMode);

	return NewCameraMode;
}

void UGeonCameraModeStack::PushCameraMode(TSubclassOf<UGeonCameraMode>& CameraModeClass)
{
	if (!CameraModeClass)
	{
		return;
	}

	UGeonCameraMode* CameraMode = GetCameraModeInstance(CameraModeClass);
	check(CameraMode);


	int32 StackSize = CameraModeStack.Num();
	if ((StackSize > 0) && (CameraModeStack[0] == CameraMode))
	{
		// CameraModeStack[0] 가장 최근에 이미 CameraMode가 Stacking되었으므로 그냥 리턴!
		return;
	}

	// ExistingStackIndex는 CameraModeStack에서 CameraMode에 맞는 Index를 찾음
	// ExistingStackContribution은 위에서 아래로 최종 BlendWeight 값을 찾기 위해 초기값으로 1.0으로 설정
	int32 ExistingStackIndex = INDEX_NONE;
	float ExistingStackContribution = 1.0f;


	/**
	 * BlendWeight    |    ExistingStackCOntribution    |    ExistingStackCOntribution (accumulated)
	 * 0.1f           |    (1.0f) * 0.1f = 0.1f         |    (1.0f - 0.1f) = 0.9f
	 * 0.3f           |    (0.9f) * 0.3f = 0.27f        |    (1.0f - 0.3f) * 0.9f = 0.63f
	 * 0.6f           |    (0.63f) * 0.6f = 0.378f      |    (1.0f - 0.6f) * 0.63f = 0.252f
	 * 1.0f           |    (0.252f) * 1.0f = 0.252f     |
	 *                |    0.1f + 0.27f + 0.378f + 0.252f = 1.0f!
	 */
	for (int32 StackIndex = 0; StackIndex < StackSize; ++StackIndex)
	{
		if (CameraModeStack[StackIndex] == CameraMode)
		{
			ExistingStackIndex = StackIndex;
			// BlendWeight를 CameraMode에 추가해주자:
			// - 여기서 ExistingStackContribution 우리가 찾는 CameraMode를 찾았으니깐, 누적된 BlendWeight와 함께 BlendWeight을 곱하여, 루프를 빠져나온다
			ExistingStackContribution *= CameraMode->BlendWeight;
			break;
		}
		else
		{
			// 당연히 우리가 원하는 CamearMode가 아니니깐, InvBlendWeight = (1.0 - BlendWeight)를 곱해줘야, 값이 누적되겠징?
			ExistingStackContribution *= (1.0f - CameraModeStack[StackIndex]->BlendWeight);
		}
	}
	// 해당 루프의 동작 원리는 앞서 적어놓은 표를 확인해보며 이해해보자.

	// 우리는 CameraMode를 Top으로 반영하기 위해, 당연히 중간에 있었다면, 제거하여 다시 Push 해줘야 한다
	if (ExistingStackIndex != INDEX_NONE)
	{
		CameraModeStack.RemoveAt(ExistingStackIndex);
		StackSize--;
	}
	else
	{
		// 없었으니깐 당연히 Contribution은 0으로 세팅해줘야겠징?
		ExistingStackContribution = 0.0f;
	}

	// BlendTime이 0보다 크다는 것은 Blend을 얼마 시간동안 진행함을 의미 따라서, ExistingStackContribution을 적용
	// - 따라서 Blend하지 않는다면, BlendWeight를 1.0을 넣어 새로 넣는 CameraMode만 적용할 것이다
	const bool bShouldBlend = ((CameraMode->BlendTime > 0.f) && (StackSize > 0));
	const float BlendWeight = (bShouldBlend ? ExistingStackContribution : 1.0f);
	CameraMode->BlendWeight = BlendWeight;

	// 흠.. Array를 Stack처럼 사용하는것은 알지만, Index 0에 넣는건 비효율적인데..
	// - Push, Pop 메서드와 같이그냥 last에 넣는게... 어떨까 싶음
	CameraModeStack.Insert(CameraMode, 0);

	// 앞서 설명했듯이 마지막은 항상 1.0이 되어야 함!
	CameraModeStack.Last()->BlendWeight = 1.0f;
}

void UGeonCameraModeStack::EvaluateStack(float DeltaTime, FGeonCameraModeView& OutCameraModeView)
{
	// Top -> Bottom [0 -> Num]까지 순차적으로 Stack에 있는 CameraMode 업데이트
	UpdateStack(DeltaTime);

	// Bottom -> Top까지 CameraModeStack에 대해 Blending 진행
	BlendStack(OutCameraModeView);
}

void UGeonCameraModeStack::UpdateStack(float DeltaTime)
{
	const int32 StackSize = CameraModeStack.Num();
	if (StackSize <= 0)
	{
		return;
	}

	// CameraModeStack을 순회하며, CameraMode를 업데이트한다
	int32 RemoveCount = 0;
	int32 RemoveIndex = INDEX_NONE;
	for (int32 StackIndex = 0; StackIndex < StackSize; ++StackIndex)
	{
		UGeonCameraMode* CameraMode = CameraModeStack[StackIndex];
		check(CameraMode);

		// UpdateCameraMode를 확인해보자:
		CameraMode->UpdateCameraMode(DeltaTime);

		// 만약 하나라도 CameraMode가 BlendWeight가 1.0에 도달했다면, 그 이후 CameraMode를 제거한다
		if (CameraMode->BlendWeight >= 1.0f)
		{
			RemoveIndex = (StackIndex + 1);
			RemoveCount = (StackSize - RemoveIndex);
			break;
		}
	}

	if (RemoveCount > 0)
	{
		// 생각해보면 이거 때문에 Pop하고 Push를 안한거일지도?
		CameraModeStack.RemoveAt(RemoveIndex, RemoveCount);
	}
}

void UGeonCameraModeStack::BlendStack(FGeonCameraModeView& OutCameraModeView) const
{
}

 