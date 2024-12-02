// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "Components/PawnComponent.h"
#include "GeonHeroComponent.generated.h"

/**
  카메라, 입력 등 플레이어가 제어하는 시스템의 초기화를 처리하는 컴포넌트 
 */
UCLASS(Blueprintable, Meta = (BlueprintSpawnableComponent))
class GEONGAME_API UGeonHeroComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()

public:
	UGeonHeroComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
};
