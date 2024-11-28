// Fill out your copyright notice in the Description page of Project Settings.


#include "GeonExperienceManagerComponent.h"

void UGeonExperienceManagerComponent::CallOrRegister_OnExperienceLoaded(FOnGeonExperienceLoaded::FDelegate&& Delegate)
{
	// ����� ������ Experience Loading�� �Ϸᰡ �Ǿ��� ���� �ְ� �ȵǾ��� ���� �ִ�.
	// ���� �Ϸᰡ �Ǿ��ٸ� �ٷ� ����, �ƴ϶�� Delegate�� �ɾ��ش�. 

	// IsExperienceLoaded() ����
	if (IsExperienceLoaded())
	{
		Delegate.Execute(CurrentExperience);
	}
	else
	{
		// ���� �ּ�����. Ȯ���� ��  �������� ���߱� ���� Move�� ���� �ϴ� ���� �������ڴ°�..
		// modern c++ �� �����ε� �ڼ��� �˰� ������ ������ ����, �ް� ����, ���� ������ �˻��� ���ƶ� 
		OnExperienceLoaded.Add(MoveTemp(Delegate));
	}
}
