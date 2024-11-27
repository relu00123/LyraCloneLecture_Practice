// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "GeonAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class GEONGAME_API UGeonAssetManager : public UAssetManager
{
	GENERATED_BODY() 

public:
	UGeonAssetManager();

	static UGeonAssetManager& Get();

	virtual void StartInitialLoading() final;

	// �ش� �Լ��� FScopeLogTime�� ������� ������ ���� �Ǵ��� ���� ���
	// ���� Ŭ���ڵ��� �ʿ� ������, �˾Ƶθ� ���� ������ ������ �غ���..!
	static bool ShouldLogAssetLoads();

	static UObject* synchronousLoadAsset(const FSoftObjectPath& AssetPath);

	// GetAsset�� ���� �ε����� BP Class�� Object �ε�
	// �ָ��ؾ� �� �κ��� bKeepInMemory
	template <typename AssetType>
	static AssetType* GetAsset(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepInMemory = true);

	template <typename AssetType>
	static TSubclassOf<AssetType> GetSubclass(const TSoftClassPtr<AssetType>& AssetPointer, bool bKeepInMemory = true);


	// [THREAD_SAFE] �޸𸮿� �ε��� ���� �ɽ�
	void AddLoadedAsset(const UObject* Asset);

	// GC�� ���
	UPROPERTY()
	TSet<TObjectPtr<const UObject>> LoadedAssets;  // UObjectPtr�� Set���� ������ 

	// Object ���� Locking
	FCriticalSection SyncObject; 
};

template<typename AssetType>
AssetType* UGeonAssetManager::GetAsset(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepsInMemory)
{
	AssetType* LoadedAsset = nullptr;
	const FSoftObjectPath& AssetPath = AssetPointer.ToSoftObjectPath();
	if (AssetPath.IsValid())
	{
		LoadedAsset = AssetPointer.Get();
		if (!LoadedAsset)
		{
			LoadedAsset = Cast<AssetType>(synchronousLoadAsset(AssetPath));
			ensureAlwaysMsgf(LoadedAsset, TEXT("Failed to load asset [%s]"), *AssetPointer.ToString());
		}

		if (LoadedAsset && bKeepsInMemory)
		{
			// ���⼭ AddLoadAsset�� �޸𸮿� �����ϱ� ���� ��ġ��� �����ϸ� ��.
			// - �ѹ� ��ϵǸ� ���� ������ �ʴ��� unload�� ���� ���� ( == ĳ��)
			Get().AddLoadedAsset(Cast<UObject>(LoadedAsset));
		}
	}

	return LoadedAsset;
}

template<typename AssetType>
inline TSubclassOf<AssetType> UGeonAssetManager::GetSubclass(const TSoftClassPtr<AssetType>& AssetPointer, bool bKeepInMemory)
{
	TSubclassOf<AssetType> LoadedSubclass;
	const FSoftObjectPath& AssetPath = AssetPointer.ToSoftObjectPath();
	if (AssetPath.IsValid())
	{
		// �ε��� �Ǿ��ִ�? -> �ٷ� �����´�. 
		// �ε��� �ȵǾ� �ִ� -> Null�� ��ȯ�Ѵ� 
		LoadedSubclass = AssetPointer.Get();
		if (!LoadedSubclass)
		{
			// ���� �ε��� �Ǿ� ���� �ʴ� -> ���� �ε��� �������ش�. 
			LoadedSubclass = Cast<UClass>(synchronousLoadAsset(AssetPath));
			ensureAlwaysMsgf(LoadedSubclass, TEXT("Failed to load asset class [%s]"), *AssetPointer.ToString());
		}

		// bKeepInMemory�� default���� true. 
		if (LoadedSubclass && bKeepInMemory)
		{
			// AddLoadedAsset�� ���������� Lock�� �ɰ� �ִ�. 
			Get().AddLoadedAsset(Cast<UObject>(LoadedSubclass));
		}
	}

	return LoadedSubclass;
}
