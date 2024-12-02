// Fill out your copyright notice in the Description page of Project Settings.


#include "GeonAssetManager.h"
#include "GeonGame/GeonLogChannels.h"
#include "GeonGame/GeonGameplayTags.h"

 

UGeonAssetManager::UGeonAssetManager()
{
}

UGeonAssetManager& UGeonAssetManager::Get()
{
	check(GEngine);

	// 우리는 AssetManager를 UEngine의 GEngine의 AssetManager의 Class를 오버라이드 했기 때문에, GEngine에 Asset Manager아 있음
	if (UGeonAssetManager* Singleton = Cast<UGeonAssetManager>(GEngine->AssetManager))
	{
		UE_LOG(LogGeon, Log, TEXT("AssetManager successfully casted: %s"), *Singleton->GetName());
		return *Singleton; // 역참조를 통해 실제 객체를 반환한다. 
	}

	UE_LOG(LogGeon, Fatal, TEXT("Invalid AssetManagerClassname in DefaultEngine.ini(project settings); it must be GeonAssetManager"));

	// 위의 UE_LOG의 Fatal로 인해, Crash가 나기 때문에 도달하지 않지만 컴파일을 위해 더미로 리턴
	return *NewObject<UGeonAssetManager>();
}

PRAGMA_DISABLE_OPTIMIZATION
void UGeonAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FGeonGameplayTags::InitializeNativeTags();
}
PRAGMA_ENABLE_OPTIMIZATION

bool UGeonAssetManager::ShouldLogAssetLoads()
{
	// 커멘드라인은 커멘드 인자로 받는 arg[1], arg[2] 등을 FCommandLine::Get()을 통해 가져올 수 있다. 
	const TCHAR* CommandLineContent = FCommandLine::Get();
	static bool bLogAssetLoads = FParse::Param(CommandLineContent, TEXT("LogAssetLoads"));
	return bLogAssetLoads;
}

// 동기식으로 로딩을 한다 ...! 기존에 있던 코드의 래퍼함수 
UObject* UGeonAssetManager::synchronousLoadAsset(const FSoftObjectPath& AssetPath)
{
	// 이 함수를 따로 만드는 이유는 'synchronous load asset이 불필요하게 하는 것이 없는지 확인하기 위함'
	if (AssetPath.IsValid())
	{
		// FScopeLogTime을 확인해보자
		TUniquePtr<FScopeLogTime> LogTimePtr;
		if (ShouldLogAssetLoads())
		{
			 
			// 단순히 로깅하면서, 초단위로 로깅 진행
			LogTimePtr = MakeUnique<FScopeLogTime>(*FString::Printf(TEXT("synchronous loaded assets [%s]"), *AssetPath.ToString()),
				nullptr, FScopeLogTime::ScopeLog_Seconds);
		}

		// 여기서 두가지 분기 :
		// 1. AssetManager 가 있다면 , AssetManager의 StreamableManager를 통해 정적 로딩
		// 2. 아니면, FSoftObjectPath를 통해 바로 정적 로딩
		if (UAssetManager::IsValid())
		{
			return UAssetManager::GetStreamableManager().LoadSynchronous(AssetPath);
		}

		// if asset manager is not ready, use LoadObject()
		// - 슥 보면, StaticLoadObject가 보인다 :
		// - 참고로, 항상 StaticLoadObject하기 전에 StaticFindObject를 통해 확인하고 실패하면 진짜 로딩함
		return AssetPath.TryLoad();
	}

	return nullptr;
}

void UGeonAssetManager::AddLoadedAsset(const UObject* Asset)
{
	if (ensureAlways(Asset))
	{
		FScopeLock Lock(&SyncObject);
		LoadedAssets.Add(Asset);
	}
}
