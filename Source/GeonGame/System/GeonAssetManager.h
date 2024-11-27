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

	// 해당 함수는 FScopeLogTime을 사용하지 않을지 유무 판단을 위해 사용
	// 굳이 클론코딩할 필요 없지만, 알아두면 좋은 점들이 있으니 해보자..!
	static bool ShouldLogAssetLoads();

	static UObject* synchronousLoadAsset(const FSoftObjectPath& AssetPath);

	// GetAsset은 정적 로딩으로 BP Class와 Object 로딩
	// 주목해야 할 부분은 bKeepInMemory
	template <typename AssetType>
	static AssetType* GetAsset(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepInMemory = true);

	template <typename AssetType>
	static TSubclassOf<AssetType> GetSubclass(const TSoftClassPtr<AssetType>& AssetPointer, bool bKeepInMemory = true);


	// [THREAD_SAFE] 메모리에 로딩된 에셋 케싱
	void AddLoadedAsset(const UObject* Asset);

	// GC의 대상
	UPROPERTY()
	TSet<TObjectPtr<const UObject>> LoadedAssets;  // UObjectPtr을 Set으로 저장함 

	// Object 단위 Locking
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
			// 여기서 AddLoadAsset은 메모리에 상주하기 위한 장치라고 생각하면 됨.
			// - 한번 등록되면 직접 내리지 않는한 unload가 되지 않음 ( == 캐싱)
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
		// 로딩이 되어있다? -> 바로 가져온다. 
		// 로딩이 안되어 있다 -> Null을 반환한다 
		LoadedSubclass = AssetPointer.Get();
		if (!LoadedSubclass)
		{
			// 아직 로딩이 되어 있지 않다 -> 동기 로딩을 진행해준다. 
			LoadedSubclass = Cast<UClass>(synchronousLoadAsset(AssetPath));
			ensureAlwaysMsgf(LoadedSubclass, TEXT("Failed to load asset class [%s]"), *AssetPointer.ToString());
		}

		// bKeepInMemory의 default값은 true. 
		if (LoadedSubclass && bKeepInMemory)
		{
			// AddLoadedAsset은 내부적으로 Lock을 걸고 있다. 
			Get().AddLoadedAsset(Cast<UObject>(LoadedSubclass));
		}
	}

	return LoadedSubclass;
}
