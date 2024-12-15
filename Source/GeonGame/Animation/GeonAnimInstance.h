// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GeonAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class GEONGAME_API UGeonAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	/** �ش� �Ӽ����� Lyra�� AnimBP���� ���Ǵ� ���̹Ƿ� ���������� */
	UPROPERTY(BlueprintReadOnly, Category = "Character State Data")
	float GroundDistance = -1.0f;
	
};
