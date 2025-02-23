// INVI_1998 All Rights Reserved.


#include "AI/BTService_OrientToTargetActor.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "Kismet/KismetMathLibrary.h"

UBTService_OrientToTargetActor::UBTService_OrientToTargetActor()
{
	NodeName = TEXT("朝向目标旋转");	// 设置节点名称：本地旋转到目标角色

	INIT_SERVICE_NODE_NOTIFY_FLAGS();	// 初始化服务节点通知标志

	RotationInterpSpeed = 5.f;		// 旋转插值速度
	Interval = 0.0f;				// 间隔：因为我们需要每帧都旋转，所以这里设置为0
	RandomDeviation = 0.0f;			// 随机偏差：因为我们不需要随机偏差，所以这里设置为0

	// 通过这个函数来添加一个对象过滤器，这个过滤器可以让我们在下拉菜单中保留我们需要的键的类型
	// GET_MEMBER_NAME_CHECKED(ThisClass, InTargetActorKey), AActor::StaticClass() 这个函数的第一个参数是我们的类，第二个参数是我们需要过滤的键，第三个参数是我们需要过滤的类型
	// 这里的意思是：我们需要过滤的是 AActor 类型的键，我们的类是 ThisClass，我们需要过滤的键是 InTargetActorKey
	InTargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, InTargetActorKey), AActor::StaticClass());
}

void UBTService_OrientToTargetActor::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	// 在这个函数中，我们需要手动解析我们的黑板键选择器
	if (UBlackboardData* BBAsset = GetBlackboardAsset())
	{
		InTargetActorKey.ResolveSelectedKey(*BBAsset);	// 解析选择的键
	}
}

FString UBTService_OrientToTargetActor::GetStaticDescription() const
{
	const FString KeyDescription = InTargetActorKey.SelectedKeyName.ToString();
	return FString::Printf(TEXT("Orient rotation to %s key %s"), *KeyDescription, *GetStaticServiceDescription());
}

void UBTService_OrientToTargetActor::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// 差值旋转
	if (AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(InTargetActorKey.SelectedKeyName)))
	{
		if (APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn())
		{
			// FRotator TargetRotation = (TargetActor->GetActorLocation() - ControlledPawn->GetActorLocation()).Rotation();
			// ControlledPawn->SetActorRotation(FMath::RInterpTo(ControlledPawn->GetActorRotation(), TargetRotation, DeltaSeconds, RotationInterpSpeed));

			const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(ControlledPawn->GetActorLocation(), TargetActor->GetActorLocation());
			ControlledPawn->SetActorRotation(FMath::RInterpTo(ControlledPawn->GetActorRotation(), LookAtRotation, DeltaSeconds, RotationInterpSpeed));
		}
	}
}
