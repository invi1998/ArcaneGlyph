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
	return GetAbilitySystemComponent();
}

void AArcaneCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (ArcaneAbilitySystemComponent)
	{
		ArcaneAbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}




