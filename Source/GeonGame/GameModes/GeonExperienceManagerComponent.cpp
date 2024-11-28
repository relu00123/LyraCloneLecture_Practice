// Fill out your copyright notice in the Description page of Project Settings.


#include "GeonExperienceManagerComponent.h"

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
