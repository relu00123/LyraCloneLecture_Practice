// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputTriggers.h"
#include "InputActionValue.h"
#include "GeonInputConfig.h"
#include "EnhancedInputComponent.h"
#include "GeonInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class GEONGAME_API UGeonInputComponent : public UEnhancedInputComponent  
{
	GENERATED_BODY()

public:
	UGeonInputComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	 * member methods
	 */
	template <class UserClass, typename FuncType>
	void BindNativeAction(const UGeonInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func, bool bLogIfNotFound);

	template <class UserClass, typename PressedFuncType, typename ReleasedFuncType>
	void BindAbilityActions(const UGeonInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles);
	
};

template <class UserClass, typename FuncType>
void UGeonInputComponent::BindNativeAction(const UGeonInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func, bool bLogIfNotFound)
{
	check(InputConfig);

	// 여기서 알 수 있듯이, InputConfig는 활성화 가능한 InputAction을 담고 있다.
	// - 만약 InputConfig에 없는 InputAction을 Binding시키면, nullptr을 반환하여, 바인딩하는데 실패한다!
	if (const UInputAction* IA = InputConfig->FindNativeInputActionForTag(InputTag, bLogIfNotFound))
	{
		BindAction(IA, TriggerEvent, Object, Func);
	}
}

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType>
void UGeonInputComponent::BindAbilityActions(const UGeonInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles)
{
	check(InputConfig);

	// AbilityAction에 대해서는 그냥 모든 InputAction에 다 바인딩 시킨다!
	for (const FGeonInputAction& Action : InputConfig->AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				BindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, PressedFunc, Action.InputTag).GetHandle());
			}

			if (ReleasedFunc)
			{
				BindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag).GetHandle());
			}
		}
	}
}

