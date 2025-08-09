// INVI_1998 All Rights Reserved.


#include "AbilitySystem/Abilities/ArcaneHeroStealthAbility.h"

#include "Characters/ArcaneHeroCharacter.h"
#include "Characters/PlayerPhantom.h"
#include "Controllers/ArcaneHeroController.h"

UArcaneHeroStealthAbility::UArcaneHeroStealthAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UArcaneHeroStealthAbility::SpawnStealthPhantom()
{
	AArcaneHeroCharacter* HeroCharacter = GetHeroCharacterFromActorInfo();
	AArcaneHeroController* HeroController = GetHeroControllerFromActorInfo();
	if (!HeroCharacter || !HeroController || !PhantomClass)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	// 保存原始团队 ID
	OriginalTeamID = HeroController->GetGenericTeamId();

	// 设置玩家为中立团队（AI不攻击）
	HeroController->SetGenericTeamId(FGenericTeamId(2)); // 2 是中立团队 ID

	// 生成幻影
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnedPhantom = GetWorld()->SpawnActor<APlayerPhantom>(
		PhantomClass,
		HeroCharacter->GetActorLocation(),
		HeroCharacter->GetActorRotation(),
		SpawnParams);

	if (SpawnedPhantom)
	{
		// 初始化幻影
		SpawnedPhantom->InitializePhantom(HeroCharacter, PhantomLifetime);

		// 设置幻影的团队 ID 为中立团队
		SpawnedPhantom->SetGenericTeamId(FGenericTeamId(2));

		// 设置10秒后销毁幻影
		
	}
	else
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}
	
}
