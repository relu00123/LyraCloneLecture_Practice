#include "GameFeatureAction_WorldActionBase.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(GameFeatureAction_WorldActionBase)

void UGameFeatureAction_WorldActionBase::OnGameFeatureActivating(FGameFeatureActivatingContext& Context)
{
	// ���带 ��ȸ�ϸ鼭,
	for (const FWorldContext& WorldContext : GEngine->GetWorldContexts())
	{
		// �ռ�, ExperienceManagerComponent���� GameFeatureAction�� Ȱ��ȭ�ϸ鼭, Context�� World�� �־���
		// - �̸� ���� ������ ������� �Ǵ�
		if (Context.ShouldApplyToWorldContext(WorldContext))
		{
			// WorldActionBase�� Interface�� AddToWorld ȣ��
			AddToWorld(WorldContext, Context);
		}
	}
}