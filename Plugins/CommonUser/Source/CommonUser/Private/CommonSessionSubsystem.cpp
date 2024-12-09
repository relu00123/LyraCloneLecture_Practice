// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonSessionSubsystem.h"
#include "Engine/AssetManager.h"
#include "Engine/LocalPlayer.h"

FString UCommonSession_HostSessionRequest::GetMapName() const
{
	// AssetManager를 통해, MapID를 통해, FAssetData를 반환하고, 이를 통해 PackageName를 통해 Map경로를 반환한다.
	FAssetData MapAssetData;
	if (UAssetManager::Get().GetPrimaryAssetData(MapID, MapAssetData)) // MapId에 해당하는 에셋만 가져온다. 
	{
		return MapAssetData.PackageName.ToString(); // 그 에셋의 이름을 반환한다. 
	}

	return FString();
}

FString UCommonSession_HostSessionRequest::ConstructTravelURL() const
{
	FString CombinedExtraArgs;

	for (const auto& ExtraArg : ExtraArgs) {

		if (ExtraArg.Key.IsEmpty())
			continue;

		// ? 를 Seperate로 복수개의 ExtraArgs를 추가함 :
		// Key 값 유무에 따라, =(assignment)를 통해 알맞는 CmdArg를 생성
		if (ExtraArg.Value.IsEmpty())
		{
			CombinedExtraArgs += FString::Printf(TEXT("?%s"), *ExtraArg.Key);
		}
		else
		{
			CombinedExtraArgs += FString::Printf(TEXT("?%s=%s"), *ExtraArg.Key, *ExtraArg.Value);
		}
	}

	return FString::Printf(TEXT("%s%s"), *GetMapName(), *CombinedExtraArgs);
}

void UCommonSessionSubsystem::HostSession(APlayerController* HostingPlayer, UCommonSession_HostSessionRequest* Request)
{
	ULocalPlayer* LocalPlayer = (HostingPlayer != nullptr) ? HostingPlayer->GetLocalPlayer() : nullptr; 
	if (!LocalPlayer)
		return;

	// ABC?Experience=FPSMode 
	// HostSessionRequest에서 MapID와 ExtraArgs를 통해 URL을 생성항, MapLoad를 시작한다.
	GetWorld()->ServerTravel(Request->ConstructTravelURL());
}
