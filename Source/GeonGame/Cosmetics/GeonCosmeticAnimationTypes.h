// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GeonCosmeticAnimationTypes.generated.h"

USTRUCT(BlueprintType)
struct FGeonAnimBodyStyleSelectionEntry
{
	GENERATED_BODY()

	/** AnimLayer�� ������ ��� SkeletalMesh */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMesh> Mesh = nullptr;

	/** Cosmetic Tag��� �����ϸ� �� */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Cosmetic"))
	FGameplayTagContainer RequiredTags;
};


USTRUCT(BlueprintType)
struct FGeonAnimBodyStyleSelectionSet
{
	GENERATED_BODY()

	/** AnimLayer ������ SkeletalMesh�� ��� ���� -> Animation-Mesh�� Rules�� MeshRules��� �����ϸ� �� */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FGeonAnimBodyStyleSelectionEntry> MeshRules;

	/** �׳� ����Ʈ�� ������ SkeletalMesh */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMesh> DefaultMesh = nullptr;

	/** Physics Asset�� �ϳ��� ������ -> �� ��� Animation�� Physics �Ӽ��� ������ */
	UPROPERTY(EditAnywhere)
	TObjectPtr<UPhysicsAsset> ForcedPhysicsAsset = nullptr;
};