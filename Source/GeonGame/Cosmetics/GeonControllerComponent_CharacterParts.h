// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GeonCharacterPartTypes.h"
#include "Components/ControllerComponent.h"
#include "GeonControllerComponent_CharacterParts.generated.h"

class UGeonPawnComponent_CharacterParts;

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

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UGeonPawnComponent_CharacterParts* GetPawnCustomizer() const;

	UFUNCTION(BlueprintCallable, Category = Cosmetics)
	void AddCharacterPart(const FGeonCharacterPart& NewPart);

	void AddCharacterPartInternal(const FGeonCharacterPart& NewPart);

	void RemoveAllCharacterParts();

	/** UFUNCTION���� �������� ������ AddDynamic�� ���� �ʴ´�! */
	UFUNCTION()
	void OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn);

	UPROPERTY(EditAnywhere, Category = Cosmetics)
	TArray<FGeonControllerCharacterPartEntry> CharacterParts;
};