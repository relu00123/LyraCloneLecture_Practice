// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "Components/PawnComponent.h"
#include "GeonHeroComponent.generated.h"

/**
  ī�޶�, �Է� �� �÷��̾ �����ϴ� �ý����� �ʱ�ȭ�� ó���ϴ� ������Ʈ 
 */
UCLASS(Blueprintable, Meta = (BlueprintSpawnableComponent))
class GEONGAME_API UGeonHeroComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()

public:
	UGeonHeroComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
};
