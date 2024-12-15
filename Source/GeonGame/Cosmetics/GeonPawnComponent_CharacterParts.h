// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GeonCharacterPartTypes.h"
#include "Components/PawnComponent.h"
#include "GeonCosmeticAnimationTypes.h"
#include "GeonPawnComponent_CharacterParts.generated.h"

class UGeonPawnComponent_CharacterParts;

/** �ν��Ͻ�ȭ �� Character Part�� ���� */
USTRUCT()
struct FGeonAppliedCharacterPartEntry
{
	GENERATED_BODY()

	/** Character Part�� ����(��Ÿ ������) */
	UPROPERTY()
	FGeonCharacterPart Part;

	/** GeonCharacterPartList���� �Ҵ� ���� Part �ڵ鰪 (FGeonControllerCharacterPartEntry�� Handle ���� ���ƾ� �� -> ������ ���� Part) */
	UPROPERTY()
	int32 PartHandle = INDEX_NONE;

	/** �ν��Ͻ�ȭ �� Character Part�� Actor */
	UPROPERTY()
	TObjectPtr<UChildActorComponent> SpawnedComponent = nullptr;
};


/** GeonPawnComponent_CharacterParts���� ������ Character Parts�� �����ϴ� Ŭ���� */
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

	/** ���� �ν��Ͻ�ȭ�� Character Part */
	UPROPERTY()
	TArray<FGeonAppliedCharacterPartEntry> Entries;

	/** �ش� GeonCharacterPartList�� Owner�� PawnComponent */
	UPROPERTY()
	TObjectPtr<UGeonPawnComponent_CharacterParts> OwnerComponent;

	/** �ռ� ���Ҵ� PartHandle�� ���� �Ҵ� �� �����ϴ� ���� */
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
	
	// �ν��Ͻ�ȭ�� Character Parts
	UPROPERTY()
	FGeonCharacterPartList CharacterPartList;

	// ���ϸ��̼� ������ ���� �޽ÿ� �����
	UPROPERTY(EditAnywhere, Category = Cosmetics)
	FGeonAnimBodyStyleSelectionSet BodyMeshes;
};
