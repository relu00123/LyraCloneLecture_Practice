// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GeonCameraMode.h"
#include "Camera/CameraComponent.h"
#include "GeonCameraComponent.generated.h"

class UGeonCameraModeStack;

template <class TClass> class TSubclassOf;

/** (return type, delegate_name) */
DECLARE_DELEGATE_RetVal(TSubclassOf<UGeonCameraMode>, FGeonCameraModeDelegate);

/**
 * 
 */
UCLASS()
class GEONGAME_API UGeonCameraComponent : public UCameraComponent
{
	GENERATED_BODY()

public:
	UGeonCameraComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	static UGeonCameraComponent* FindCameraComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UGeonCameraComponent>() : nullptr); }

	// Member Methods 
	AActor* GetTargetActor() const { return GetOwner(); }
	void UpdateCameraModes();

	// CameraComponent interface
	virtual void OnRegister() final;
	virtual void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) final;

	 

	UPROPERTY()
	TObjectPtr<UGeonCameraModeStack> CameraModeStack;

	/** 현재 CameraMode를 가져오는 Delegate */
	FGeonCameraModeDelegate DetermineCameraModeDelegate;

	
};
