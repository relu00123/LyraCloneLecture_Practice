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

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UGeonPawnComponent_CharacterParts* GetPawnCustomizer() const;

	UFUNCTION(BlueprintCallable, Category = Cosmetics)
	void AddCharacterPart(const FGeonCharacterPart& NewPart);

	void AddCharacterPartInternal(const FGeonCharacterPart& NewPart);

	void RemoveAllCharacterParts();

	/** UFUNCTION으로 정의하지 않으면 AddDynamic이 되지 않는다! */
	UFUNCTION()
	void OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn);

	UPROPERTY(EditAnywhere, Category = Cosmetics)
	TArray<FGeonControllerCharacterPartEntry> CharacterParts;
};