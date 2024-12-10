// Fill out your copyright notice in the Description page of Project Settings.


#include "GeonExperienceManagerComponent.h"
#include "GeonGame/System/GeonAssetManager.h"
#include "GeonGame/GameModes/GeonExperienceDefinition.h"
#include "GameFeaturesSubsystem.h"
#include "GeonExperienceActionSet.h"
#include "GameFeatureAction.h"
#include "GameFeaturesSubsystemSettings.h"

void UGeonExperienceManagerComponent::CallOrRegister_OnExperienceLoaded(FOnGeonExperienceLoaded::FDelegate&& Delegate)
{
	// 물어보는 시점에 Experience Loading이 완료가 되었을 수도 있고 안되었을 수도 있다.
	// 만약 완료가 되었다면 바로 실행, 아니라면 Delegate를 걸어준다. 

	// IsExperienceLoaded() 구현
	if (IsExperienceLoaded())
	{
		Delegate.Execute(CurrentExperience);
	}
	else
	{
		// 여기 주석있음. 확인할 것  복사비용을 낮추기 위해 Move를 통해 하는 것을 잊지말자는것..
		// modern c++ 의 개념인데 자세히 알고 싶으면 오름값 참조, 왼값 참조, 보편 참조를 검색해 보아라 
		OnExperienceLoaded.Add(MoveTemp(Delegate));
	}
}

void UGeonExperienceManagerComponent::ServerSetCurrentExperience(FPrimaryAssetId ExperienceId)
{
	UGeonAssetManager& AssetManager = UGeonAssetManager::Get();

	TSubclassOf<UGeonExperienceDefinition> AssetClass;
	{
		FSoftObjectPath AssetPath = AssetManager.GetPrimaryAssetPath(ExperienceId);
		AssetClass = Cast<UClass>(AssetPath.TryLoad());
	}

	// 왜 CDO를 가져오는 걸까?
	const UGeonExperienceDefinition* Experience = GetDefault<UGeonExperienceDefinition>(AssetClass);
	check(Experience != nullptr);
	check(CurrentExperience == nullptr);
	{
		// 그리고 CDO로 CurrentExperience를 설정한다!
		// 어떤 의도로 이렇게 코드를 작성한지는 코드를 쭉 읽어보고(StartExperienceLoad까지 읽어보자) 다시 생각해보자:
		CurrentExperience = Experience;
	}

	StartExperienceLoad();
}

void UGeonExperienceManagerComponent::StartExperienceLoad()
{
	check(CurrentExperience);
	check(LoadState == EGeonExperienceLoadState::Unloaded);

	LoadState = EGeonExperienceLoadState::Loading;

	UGeonAssetManager& AssetManager = UGeonAssetManager::Get();

	// 이미 앞서, ServerSetCurrentExperience에서 우리는 ExperienceId를 넘겨주었는데, 여기서 CDO를 활용하여, GetPrimaryAssetId를 로딩할 대상으로 넣는다!
	// - 왜 이렇게 하는걸까?
	// - GetPrimaryAssetId를 좀 더 자세히보자:
	// - GetPrimaryAssetId를 살펴봄으로써, 아래의 두가지를 알 수 있다:
	//   1. 우리는 B_GeonDefaultExperience를 BP로 만든 이유
	//   2. CDO를 가져와서, GetPrimaryAssetId를 호출한 이유

	// 우리는 앞서 이미 CDO로 로딩하여, CDO를 사용하지 않고 CDO를 사용하여 로딩할 에셋을 지정하여, BundleAssetList에 추가해준다!

	TSet<FPrimaryAssetId> BundleAssetList;
	BundleAssetList.Add(CurrentExperience->GetPrimaryAssetId());

	// load assets associated with the experience
	// 아래는 우리가 후일 GameFeature를 사용하여, Experience에 바인딩된 GameFeature Plugin을 로딩할 Bundle 이름을 추가한다:
	// - Bundle이라는게 후일 우리가 로딩할 에셋의 카테고리 이름이라고 생각하면 된다 (일단 지금은 넘어가자 후일, 또 다룰 것이다!)
	TArray<FName> BundlesToLoad;
	{
		// 여기서 주목해야 할 부분은 OwnerNetMode가 NM_Standalone이면? Client/Server 둘다 로딩에 추가된다!
		const ENetMode OwnerNetMode = GetOwner()->GetNetMode();
		bool bLoadClient = GIsEditor || (OwnerNetMode != NM_DedicatedServer);
		bool bLoadServer = GIsEditor || (OwnerNetMode != NM_Client);
		if (bLoadClient)
		{
			BundlesToLoad.Add(UGameFeaturesSubsystemSettings::LoadStateClient);
		}
		if (bLoadServer)
		{
			BundlesToLoad.Add(UGameFeaturesSubsystemSettings::LoadStateServer);
		}
	}

	FStreamableDelegate OnAssetsLoadedDelegate = FStreamableDelegate::CreateUObject(this, &ThisClass::OnExperienceLoadComplete);

	// 아래도, 후일 Bundle을 우리가 GameFeature에 연동하면서 더 깊게 알아보기로 하고, 지금은 앞서 B_GeonDefaultExperience를 로딩해주는 함수로 생각하자
	TSharedPtr<FStreamableHandle> Handle = AssetManager.ChangeBundleStateForPrimaryAssets(
		BundleAssetList.Array(),
		BundlesToLoad,
		{}, false, FStreamableDelegate(), FStreamableManager::AsyncLoadHighPriority);

	if (!Handle.IsValid() || Handle->HasLoadCompleted())
	{
		// 로딩이 완료되었으면, ExecuteDelegate를 통해 OnAssetsLoadedDelegate를 호출하자:
		// - 아래의 함수를 확인해보자:
		FStreamableHandle::ExecuteDelegate(OnAssetsLoadedDelegate);
	}
	else
	{
		Handle->BindCompleteDelegate(OnAssetsLoadedDelegate);
		Handle->BindCancelDelegate(FStreamableDelegate::CreateLambda([OnAssetsLoadedDelegate]()
			{
				OnAssetsLoadedDelegate.ExecuteIfBound();
			}));
	}

	// FrameNumber를 주목해서 보자
	static int32 StartExperienceLoad_FrameNumber = GFrameNumber;
}

void UGeonExperienceManagerComponent::OnExperienceLoadComplete()
{
	// FrameNumber를 주목해서 보자
	static int32 OnExperienceLoadComplete_FrameNumber = GFrameNumber;

	check(LoadState == EGeonExperienceLoadState::Loading);
	check(CurrentExperience);

	// 이전 활성화된 GameFeature Plugin의 URL을 클리어해준다
	GameFeaturePluginURLs.Reset();

	auto CollectGameFeaturePluginURLs = [This = this](const UPrimaryDataAsset* Context, const TArray<FString>& FeaturePluginList)
		{
			// FeaturePluginList를 순회하며, PluginURL을 ExperienceManagerComponent의 GameFeaturePluginURLS에 추가해준다
			for (const FString& PluginName : FeaturePluginList)
			{
				FString PluginURL;
				if (UGameFeaturesSubsystem::Get().GetPluginURLByName(PluginName, PluginURL))
				{
					This->GameFeaturePluginURLs.AddUnique(PluginURL);
				}
			}
		};

	// GameFeaturesToEnable에 있는 Plugin만 일단 활성화할 GameFeature Plugin 후보군으로 등록
	CollectGameFeaturePluginURLs(CurrentExperience, CurrentExperience->GameFeaturesToEnable);

	// GameFeaturePluginURLs에 등록된 Plugin을 로딩 및 활성화:
	NumGameFeaturePluginsLoading = GameFeaturePluginURLs.Num();
	if (NumGameFeaturePluginsLoading)
	{
		LoadState = EGeonExperienceLoadState::LoadingGameFeatures;
		for (const FString& PluginURL : GameFeaturePluginURLs)
		{
			// 매 Plugin이 로딩 및 활성화 이후, OnGameFeaturePluginLoadComplete 콜백 함수 등록
			// 해당 함수를 살펴보도록 하자
			UGameFeaturesSubsystem::Get().LoadAndActivateGameFeaturePlugin(PluginURL, FGameFeaturePluginLoadComplete::CreateUObject(this, &ThisClass::OnGameFeaturePluginLoadComplete));
		}
	}
	else
	{
		// 해당 함수가 불리는 것은 앞서 보았던 StreamableDelegateDelayHelper에 의해 불림
		OnExperienceFullLoadCompleted();
	}
}

void UGeonExperienceManagerComponent::OnGameFeaturePluginLoadComplete(const UE::GameFeatures::FResult& Result)
{
	// 매 GameFeature Plugin이 로딩될 때, 해당 함수가 콜백으로 불린다
	NumGameFeaturePluginsLoading--;
	if (NumGameFeaturePluginsLoading == 0)
	{
		// GameFeaturePlugin 로딩이 다 끝났을 경우, 기존대로 Loaded로서, OnExperienceFullLoadCompleted 호출한다
		// GameFeaturePlugin 로딩과 활성화가 끝났다면? UGameFeatureAction을 활성화해야겠지 (조금만 있다가 하장)
		OnExperienceFullLoadCompleted();
	}
}

void UGeonExperienceManagerComponent::OnExperienceFullLoadCompleted()
{
	check(LoadState != EGeonExperienceLoadState::Loaded);

	// GameFeature Plugin의 로딩과 활성화 이후, GameFeature Action들을 활성화 시키자:
	{
		LoadState = EGeonExperienceLoadState::ExecutingActions;

		// GameFeatureAction 활성화를 위한 Context 준비
		FGameFeatureActivatingContext Context;
		{
			// 월드의 핸들을 세팅해준다
			const FWorldContext* ExistingWorldContext = GEngine->GetWorldContextFromWorld(GetWorld());
			if (ExistingWorldContext)
			{
				Context.SetRequiredWorldContextHandle(ExistingWorldContext->ContextHandle);
			}
		}

		auto ActivateListOfActions = [&Context](const TArray<UGameFeatureAction*>& ActionList)
			{
				for (UGameFeatureAction* Action : ActionList)
				{
					// 명시적으로 GameFeatureAction에 대해 Registering -> Loading -> Activating 순으로 호출한다
					if (Action)
					{
						Action->OnGameFeatureRegistering();
						Action->OnGameFeatureLoading();
						Action->OnGameFeatureActivating(Context);
					}
				}
			};

		// 1. Experience의 Actions
		ActivateListOfActions(CurrentExperience->Actions);

		// 2. Experience의 ActionSets
		for (const TObjectPtr<UGeonExperienceActionSet>& ActionSet : CurrentExperience->ActionSets)
		{
			ActivateListOfActions(ActionSet->Actions);
		}
	}

	LoadState = EGeonExperienceLoadState::Loaded;
	OnExperienceLoaded.Broadcast(CurrentExperience);
	OnExperienceLoaded.Clear();
}

const UGeonExperienceDefinition* UGeonExperienceManagerComponent::GetCurrentExperienceChecked() const
{
	check(LoadState == EGeonExperienceLoadState::Loaded);
	check(CurrentExperience != nullptr);
	return CurrentExperience;
}
