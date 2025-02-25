// INVI_1998 All Rights Reserved.


#include "AI/BTTask_RotateToFaceTarget.h"

#include "BehaviorTree/BlackboardData.h"

UBTTask_RotateToFaceTarget::UBTTask_RotateToFaceTarget()
{
	NodeName = TEXT("旋转面向目标（Native）");

	bNotifyTick = true;		// 通知Tick
	bNotifyTaskFinished = true;		// 通知任务完成

	// 该变量是用于控制是否创建节点实例
	// 如果设置，则 Node 将被实例化，而不是使用与所有其他 BT 组件共享的内存块和模板
	bCreateNodeInstance = false;		// 不创建节点实例

	INIT_TASK_NODE_NOTIFY_FLAGS();	// 初始化任务节点通知标志

	TargetToFaceKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_RotateToFaceTarget, TargetToFaceKey), AActor::StaticClass());
}

void UBTTask_RotateToFaceTarget::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if (UBlackboardData* BBAsset = GetBlackboardAsset())
	{
		// 从黑板资产中解析选择的键
		TargetToFaceKey.ResolveSelectedKey(*BBAsset);
	}
}

uint16 UBTTask_RotateToFaceTarget::GetInstanceMemorySize() const
{
	// 因为我们需要存储FRotateToFaceTargetTaskMemory数据，所以需要返回这个数据的大小
	return sizeof(FRotateToFaceTargetTaskMemory);
}

FString UBTTask_RotateToFaceTarget::GetStaticDescription() const
{
	const FString KeyDesc = TargetToFaceKey.SelectedKeyName.IsNone() ? TEXT("none") : TargetToFaceKey.SelectedKeyName.ToString();
	return FString::Printf(TEXT("旋转面向目标: %s 直到旋转角度到达 %.1f 为止"), *KeyDesc, AnglePrecision);
}
