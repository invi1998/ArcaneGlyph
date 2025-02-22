// INVI_1998 All Rights Reserved.


#include "Characters/ArcaneCharacterBase.h"

#include "AbilitySystem/ArcaneAbilitySystemComponent.h"
#include "AbilitySystem/ArcaneAttributeSet.h"

AArcaneCharacterBase::AArcaneCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	GetMesh()->bReceivesDecals = false;		// 角色模型不接受贴花

	ArcaneAbilitySystemComponent = CreateDefaultSubobject<UArcaneAbilitySystemComponent>(TEXT("ArcaneAbilitySystemComponent"));
	ArcaneAttributeSet = CreateDefaultSubobject<UArcaneAttributeSet>(TEXT("ArcaneAttributeSet"));
	
}

UAbilitySystemComponent* AArcaneCharacterBase::GetAbilitySystemComponent() const
{
	// return GetAbilitySystemComponent();
	return GetArcaneAbilitySystemComponent();
}

UPawnCombatComponent* AArcaneCharacterBase::GetPawnCombatComponent() const
{
	return nullptr;
}

UPawnUIComponent* AArcaneCharacterBase::GetPawnUIComponent() const
{
	return nullptr;
}

void AArcaneCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (ArcaneAbilitySystemComponent)
	{
		ArcaneAbilitySystemComponent->InitAbilityActorInfo(this, this);

		// 确保角色启动数据有效，ensure 会在 Debug 编译模式下检查条件是否为真，如果为假则会触发断言
		// ensure(!CharacterStartupData.IsNull());
		ensureMsgf(!CharacterStartupData.IsNull(), TEXT("%s CharacterStartupData is null!"), *GetName());
	}
}




