// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GeonCharacterPartTypes.h"
#include "Components/PawnComponent.h"
#include "GeonCosmeticAnimationTypes.h"
#include "GeonPawnComponent_CharacterParts.generated.h"

class UGeonPawnComponent_CharacterParts;

/** 인스턴스화 된 Character Part의 단위 */
USTRUCT()
struct FGeonAppliedCharacterPartEntry
{
	GENERATED_BODY()

	/** Character Part의 정의(메타 데이터) */
	UPROPERTY()
	FGeonCharacterPart Part;

	/** GeonCharacterPartList에서 할당 받은 Part 핸들값 (FGeonControllerCharacterPartEntry의 Handle 값과 같아야 함 -> 같으면 같은 Part) */
	UPROPERTY()
	int32 PartHandle = INDEX_NONE;

	/** 인스턴스화 된 Character Part용 Actor */
	UPROPERTY()
	TObjectPtr<UChildActorComponent> SpawnedComponent = nullptr;
};


/** GeonPawnComponent_CharacterParts에서 실질적 Character Parts를 관리하는 클래스 */
USTRUCT(BlueprintType)
struct FGeonCharacterPartList
{
	GENERATED_BODY()

	FGeonCharacterPartList()
		: OwnerComponent(nullptr)
	{
	}

	FGeonCharacterPartList(UGeonPawnComponent_CharacterParts* InOwnerComponent)
		: OwnerComponent(InOwnerComponent)
	{
	}

	bool SpawnActorForEntry(FGeonAppliedCharacterPartEntry& Entry);
	void DestroyActorForEntry(FGeonAppliedCharacterPartEntry& Entry);

	FGeonCharacterPartHandle AddEntry(FGeonCharacterPart NewPart);
	void RemoveEntry(FGeonCharacterPartHandle Handle);

	FGameplayTagContainer CollectCombinedTags() const;

	/** 현재 인스턴스화된 Character Part */
	UPROPERTY()
	TArray<FGeonAppliedCharacterPartEntry> Entries;

	/** 해당 GeonCharacterPartList의 Owner인 PawnComponent */
	UPROPERTY()
	TObjectPtr<UGeonPawnComponent_CharacterParts> OwnerComponent;

	/** 앞서 보았던 PartHandle의 값을 할당 및 관리하는 변수 */
	int32 PartHandleCounter = 0;
};

/**
 * 
 */
UCLASS()
class GEONGAME_API UGeonPawnComponent_CharacterParts : public UPawnComponent
{
	GENERATED_BODY()

public:
	UGeonPawnComponent_CharacterParts(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


	USkeletalMeshComponent* GetParentMeshComponent() const;
	USceneComponent* GetSceneComponentToAttachTo() const;

	FGameplayTagContainer GetCombinedTags(FGameplayTag RequiredPrefix) const;
	void BroadcastChanged();

	FGeonCharacterPartHandle AddCharacterPart(const FGeonCharacterPart& NewPart);
	void RemoveCharacterPart(FGeonCharacterPartHandle Handle);
	
	// 인스턴스화된 Character Parts
	UPROPERTY()
	FGeonCharacterPartList CharacterPartList;

	// 에니메이션 적용을 위한 메시와 연결고리
	UPROPERTY(EditAnywhere, Category = Cosmetics)
	FGeonAnimBodyStyleSelectionSet BodyMeshes;
};
