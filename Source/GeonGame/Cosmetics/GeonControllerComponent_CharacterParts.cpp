//	Fill out your copyright notice in the Description page of Project Settings.

#include "GeonControllerComponent_CharacterParts.h"

#include "GeonPawnComponent_CharacterParts.h"

UGeonControllerComponent_CharacterParts::UGeonControllerComponent_CharacterParts(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UGeonControllerComponent_CharacterParts::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		if (AController* OwningController = GetController<AController>())
		{
			OwningController->OnPossessedPawnChanged.AddDynamic(this, &ThisClass::OnPossessedPawnChanged);
		}
	}
}

void UGeonControllerComponent_CharacterParts::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	RemoveAllCharacterParts();
	Super::EndPlay(EndPlayReason);
}

PRAGMA_DISABLE_OPTIMIZATION
UGeonPawnComponent_CharacterParts* UGeonControllerComponent_CharacterParts::GetPawnCustomizer() const
{
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		// 생각해보면, 우리는 앞서 GeonPawnComponent_CharacterParts를 상속받는 B_MannequinPawnCosmetics를 이미 B_Hero_ShooterMannequin에 추가하였다.
		// B_MannequinPawnCosmetics를 반환되길 기대한다
		return ControlledPawn->FindComponentByClass<UGeonPawnComponent_CharacterParts>();
	}
	return nullptr;
}
PRAGMA_ENABLE_OPTIMIZATION

void UGeonControllerComponent_CharacterParts::AddCharacterPart(const FGeonCharacterPart& NewPart)
{
	AddCharacterPartInternal(NewPart);
}

void UGeonControllerComponent_CharacterParts::AddCharacterPartInternal(const FGeonCharacterPart& NewPart)
{
	FGeonControllerCharacterPartEntry& NewEntry = CharacterParts.AddDefaulted_GetRef();
	NewEntry.Part = NewPart;

	if (UGeonPawnComponent_CharacterParts* PawnCustomizer = GetPawnCustomizer())
	{
		NewEntry.Handle = PawnCustomizer->AddCharacterPart(NewPart);
	}
}

void UGeonControllerComponent_CharacterParts::RemoveAllCharacterParts()
{
	if (UGeonPawnComponent_CharacterParts* PawnCustomizer = GetPawnCustomizer())
	{
		for (FGeonControllerCharacterPartEntry& Entry : CharacterParts)
		{
			PawnCustomizer->RemoveCharacterPart(Entry.Handle);
		}
	}
	CharacterParts.Reset();
}

void UGeonControllerComponent_CharacterParts::OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	// 이전 OldPawn에 대해서는 Character Parts를 전부 제거해주자
	if (UGeonPawnComponent_CharacterParts* OldCustomizer = OldPawn ? OldPawn->FindComponentByClass<UGeonPawnComponent_CharacterParts>() : nullptr)
	{
		for (FGeonControllerCharacterPartEntry& Entry : CharacterParts)
		{
			OldCustomizer->RemoveCharacterPart(Entry.Handle);
			Entry.Handle.Reset();
		}
	}

	// 새로운 Pawn에 대해서 기존 Controller가 가지고 있는 Character Parts를 추가해주자
	if (UGeonPawnComponent_CharacterParts* NewCustomizer = NewPawn ? NewPawn->FindComponentByClass<UGeonPawnComponent_CharacterParts>() : nullptr)
	{
		for (FGeonControllerCharacterPartEntry& Entry : CharacterParts)
		{
			check(!Entry.Handle.IsValid());
			Entry.Handle = NewCustomizer->AddCharacterPart(Entry.Part);
		}
	}
}
