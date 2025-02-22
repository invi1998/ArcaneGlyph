// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PawnUIComponent.h"
#include "HeroUIComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEquippedWeaponChangedDelegate, TSoftObjectPtr<UTexture2D>, InEquippedWeaponIcon);	// 装备武器图标变化委托

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ARCANEGLYPH_API UHeroUIComponent : public UPawnUIComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnPercentChangedDelegate OnCurrentRageChanged;	// 当前怒气值变化

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnEquippedWeaponChangedDelegate OnEquippedWeaponChanged;	// 装备武器变化
	
};
