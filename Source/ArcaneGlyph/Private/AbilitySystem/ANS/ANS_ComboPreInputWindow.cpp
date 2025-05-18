// INVI_1998 All Rights Reserved.


#include "AbilitySystem/ANS/ANS_ComboPreInputWindow.h"

#include "ArcaneBlueprintFunctionLibrary.h"
#include "ArcaneGameplayTags.h"
#include "AbilitySystem/ArcaneAbilitySystemComponent.h"

void UANS_ComboPreInputWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);	// 必须调用父类的NotifyBegin函数
	
	if (USkeletalMeshComponent* Mesh = MeshComp)
	{
		if (AActor* Owner = Mesh->GetOwner())
		{
		}
	}
}

void UANS_ComboPreInputWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);	// 必须调用父类的NotifyEnd函数
	
	
}
