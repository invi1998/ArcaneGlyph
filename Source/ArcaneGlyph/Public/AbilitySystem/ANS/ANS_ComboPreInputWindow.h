// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_ComboPreInputWindow.generated.h"

class UInputAction;
// 预声明
class UAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class ARCANEGLYPH_API UANS_ComboPreInputWindow : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,  const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	// 配置参数：允许缓冲哪些输入动作
	UPROPERTY(EditAnywhere, Category="Input")
	TArray<TObjectPtr<UInputAction>> BufferedInputActions;
	
};
