// INVI_1998 All Rights Reserved.


#include "AI/BTTask_RotateToFaceTarget.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "Kismet/KismetMathLibrary.h"

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

EBTNodeResult::Type UBTTask_RotateToFaceTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// CastInstanceNodeMemory 用于获取节点内存，其做的事情就是等效于 reinterpret_cast<T*>(NodeMemory);
	// 只不过它会断言检查 T 的大小是否小于等于 GetInstanceMemorySize() 返回的大小
	FRotateToFaceTargetTaskMemory* MyMemory = CastInstanceNodeMemory<FRotateToFaceTargetTaskMemory>(NodeMemory);
	check(MyMemory);

	// 获取拥有者
	MyMemory->OwningPawn = OwnerComp.GetAIOwner()->GetPawn();
	// 获取目标
	MyMemory->TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetToFaceKey.SelectedKeyName));
	if (!MyMemory->IsValid())
	{
		return EBTNodeResult::Failed;
	}

	if (HasReachedAnglePrecision(MyMemory->OwningPawn.Get(), MyMemory->TargetActor.Get()))
	{
		MyMemory->Reset();
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_RotateToFaceTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FRotateToFaceTargetTaskMemory* MyMemory = CastInstanceNodeMemory<FRotateToFaceTargetTaskMemory>(NodeMemory);
	if (!MyMemory->IsValid())
	{
		// 如果数据无效，则任务失败
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	if (HasReachedAnglePrecision(MyMemory->OwningPawn.Get(), MyMemory->TargetActor.Get()))
	{
		// 如果已经达到角度精度，则任务成功
		MyMemory->Reset();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	// 获取从拥有者到目标的旋转角度
	const FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(MyMemory->OwningPawn->GetActorLocation(), MyMemory->TargetActor->GetActorLocation());

	// 差值旋转
	const FRotator NewRot = FMath::RInterpTo(MyMemory->OwningPawn->GetActorRotation(), LookAtRot, DeltaSeconds, RotationInterpSpeed);
	MyMemory->OwningPawn->SetActorRotation(NewRot);
}

bool UBTTask_RotateToFaceTarget::HasReachedAnglePrecision(APawn* QueryPawn, AActor* TargetActor) const
{
	const FVector OwnerForward = QueryPawn->GetActorForwardVector();
	const FVector OwnerToTargetNormalized = (TargetActor->GetActorLocation() - QueryPawn->GetActorLocation()).GetSafeNormal();
	const float DotResult = FVector::DotProduct(OwnerForward, OwnerToTargetNormalized);
	// const float Angle = FMath::Acos(DotResult) * (180.f / PI);
	// 等同于 用于将弧度转换为角度
	const float Angle = UKismetMathLibrary::DegAcos(DotResult);
	return Angle <= AnglePrecision;
}
