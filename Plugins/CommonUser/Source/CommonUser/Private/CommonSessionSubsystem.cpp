// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonSessionSubsystem.h"
#include "Engine/AssetManager.h"
#include "Engine/LocalPlayer.h"

FString UCommonSession_HostSessionRequest::GetMapName() const
{
	// AssetManager�� ����, MapID�� ����, FAssetData�� ��ȯ�ϰ�, �̸� ���� PackageName�� ���� Map��θ� ��ȯ�Ѵ�.
	FAssetData MapAssetData;
	if (UAssetManager::Get().GetPrimaryAssetData(MapID, MapAssetData)) // MapId�� �ش��ϴ� ���¸� �����´�. 
	{
		return MapAssetData.PackageName.ToString(); // �� ������ �̸��� ��ȯ�Ѵ�. 
	}

	return FString();
}

FString UCommonSession_HostSessionRequest::ConstructTravelURL() const
{
	FString CombinedExtraArgs;

	for (const auto& ExtraArg : ExtraArgs) {

		if (ExtraArg.Key.IsEmpty())
			continue;

		// ? �� Seperate�� �������� ExtraArgs�� �߰��� :
		// Key �� ������ ����, =(assignment)�� ���� �˸´� CmdArg�� ����
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
	// HostSessionRequest���� MapID�� ExtraArgs�� ���� URL�� ������, MapLoad�� �����Ѵ�.
	GetWorld()->ServerTravel(Request->ConstructTravelURL());
}
