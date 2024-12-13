// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GeonCharacterPartTypes.h"
#include "Components/ControllerComponent.h"
#include "GeonControllerComponent_CharacterParts.generated.h"

USTRUCT() 
struct FGeonControllerCharacterPartEntry {
	GENERATED_BODY()

	// Character Part �� ���� ���� (��Ÿ ������ == MetaData)
	UPROPERTY(EditAnywhere)
	FGeonCharacterPart Part;

	// Character Part ��� (������) - Controller�� Possess�ϰ� �ִ� Pawn���� ������(�ν��Ͻ�) Character Part ��鰪
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