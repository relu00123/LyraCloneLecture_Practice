// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GeonCameraMode.h"
#include "GeonCameraMode_ThirdPerson.generated.h"


class UCurveVector;
/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class GEONGAME_API UGeonCameraMode_ThirdPerson : public UGeonCameraMode
{
	GENERATED_BODY()

	UGeonCameraMode_ThirdPerson(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


	/**
	* UGeonCameraMode's interface
	*/
	virtual void UpdateView(float DeltaTime) override;

	/**
	 * member variables
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Third Person")
	TObjectPtr<const UCurveVector> TargetOffsetCurve;
	
};
