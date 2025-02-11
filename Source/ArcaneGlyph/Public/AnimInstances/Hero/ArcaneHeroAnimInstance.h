// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/ArcaneCharacterAnimInstance.h"
#include "ArcaneHeroAnimInstance.generated.h"

class AArcaneHeroCharacter;
/**
 * 
 */
UCLASS()
class ARCANEGLYPH_API UArcaneHeroAnimInstance : public UArcaneCharacterAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|ReferenceData")
	TObjectPtr<AArcaneHeroCharacter> OwnerHeroCharacter;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bShouldEnterRelaxedState;		// 是否应该进入放松状态

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float EnterRelaxedStateTime = 5.f;		// 进入放松状态的时间

	float IdleElapsedTime = 0.f;		// 空闲经过的时间
	
};
