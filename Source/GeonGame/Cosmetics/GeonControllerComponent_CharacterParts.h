// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GeonCharacterPartTypes.h"
#include "Components/ControllerComponent.h"
#include "GeonControllerComponent_CharacterParts.generated.h"

USTRUCT() 
struct FGeonControllerCharacterPartEntry {
	GENERATED_BODY()

	// Character Part 에 대한 정의 (메타 데이터 == MetaData)
	UPROPERTY(EditAnywhere)
	FGeonCharacterPart Part;

	// Character Part 헨들 (고유값) - Controller가 Possess하고 있는 Pawn에서 생성한(인스턴스) Character Part 헨들값
	FGeonCharacterPartHandle Handle; 
};

UCLASS(meta = (BlueprintSpawnableComponent))
class UGeonControllerComponent_CharacterParts : public UControllerComponent
{
	GENERATED_BODY()
public:
	UGeonControllerComponent_CharacterParts(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditAnywhere, Category = Cosmetics)
	TArray<FGeonControllerCharacterPartEntry> CharacterParts;
};