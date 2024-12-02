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

	// �츮�� AssetManager�� UEngine�� GEngine�� AssetManager�� Class�� �������̵� �߱� ������, GEngine�� Asset Manager�� ����
	if (UGeonAssetManager* Singleton = Cast<UGeonAssetManager>(GEngine->AssetManager))
	{
		UE_LOG(LogGeon, Log, TEXT("AssetManager successfully casted: %s"), *Singleton->GetName());
		return *Singleton; // �������� ���� ���� ��ü�� ��ȯ�Ѵ�. 
	}

	UE_LOG(LogGeon, Fatal, TEXT("Invalid AssetManagerClassname in DefaultEngine.ini(project settings); it must be GeonAssetManager"));

	// ���� UE_LOG�� Fatal�� ����, Crash�� ���� ������ �������� ������ �������� ���� ���̷� ����
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
	// Ŀ�������� Ŀ��� ���ڷ� �޴� arg[1], arg[2] ���� FCommandLine::Get()�� ���� ������ �� �ִ�. 
	const TCHAR* CommandLineContent = FCommandLine::Get();
	static bool bLogAssetLoads = FParse::Param(CommandLineContent, TEXT("LogAssetLoads"));
	return bLogAssetLoads;
}

// ��������� �ε��� �Ѵ� ...! ������ �ִ� �ڵ��� �����Լ� 
UObject* UGeonAssetManager::synchronousLoadAsset(const FSoftObjectPath& AssetPath)
{
	// �� �Լ��� ���� ����� ������ 'synchronous load asset�� ���ʿ��ϰ� �ϴ� ���� ������ Ȯ���ϱ� ����'
	if (AssetPath.IsValid())
	{
		// FScopeLogTime�� Ȯ���غ���
		TUniquePtr<FScopeLogTime> LogTimePtr;
		if (ShouldLogAssetLoads())
		{
			 
			// �ܼ��� �α��ϸ鼭, �ʴ����� �α� ����
			LogTimePtr = MakeUnique<FScopeLogTime>(*FString::Printf(TEXT("synchronous loaded assets [%s]"), *AssetPath.ToString()),
				nullptr, FScopeLogTime::ScopeLog_Seconds);
		}

		// ���⼭ �ΰ��� �б� :
		// 1. AssetManager �� �ִٸ� , AssetManager�� StreamableManager�� ���� ���� �ε�
		// 2. �ƴϸ�, FSoftObjectPath�� ���� �ٷ� ���� �ε�
		if (UAssetManager::IsValid())
		{
			return UAssetManager::GetStreamableManager().LoadSynchronous(AssetPath);
		}

		// if asset manager is not ready, use LoadObject()
		// - �� ����, StaticLoadObject�� ���δ� :
		// - �����, �׻� StaticLoadObject�ϱ� ���� StaticFindObject�� ���� Ȯ���ϰ� �����ϸ� ��¥ �ε���
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
