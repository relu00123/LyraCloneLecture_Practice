// Fill out your copyright notice in the Description page of Project Settings.


#include "GeonCameraMode_ThirdPerson.h"
#include "Curves/CurveVector.h"

UGeonCameraMode_ThirdPerson::UGeonCameraMode_ThirdPerson(const FObjectInitializer& ObjectInitializer)
{
}

void UGeonCameraMode_ThirdPerson::UpdateView(float DeltaTime)
{
	FVector PivotLocation = GetPivotLocation();
	FRotator PivotRotation = GetPivotRotation();

	PivotRotation.Pitch = FMath::ClampAngle(PivotRotation.Pitch, ViewPitchMin, ViewPitchMax);

	View.Location = PivotLocation;
	View.Rotation = PivotRotation;
	View.ControlRotation = View.Rotation;
	View.FieldOfView = FieldOfView;

	// TargetOffsetCurve�� �������̵�Ǿ� �ִٸ�, Curve�� ���� �����ͼ� ���� ����
	// - Camera �������� Charater�� ��� �κп� Target���� ���� �����ϴ� ������ �����ϸ� ��
	if (TargetOffsetCurve)
	{
		const FVector TargetOffset = TargetOffsetCurve->GetVectorValue(PivotRotation.Pitch);
		View.Location = PivotLocation + PivotRotation.RotateVector(TargetOffset);
	}
}
