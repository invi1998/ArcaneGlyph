// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "FrontendUISubsystem.generated.h"

class UWidget_ActivatableBase;
struct FGameplayTag;
class UWidget_PrimaryLayout;

enum class EAsyncPushWidgetState : uint8
{
	OnCreatedBeforePush UMETA(DisplayName = "在创建推送之前"),
	AfterPush UMETA(DisplayName = "推送之后"),
};

/**
 * 
 */
UCLASS()
class ARCANEGLYPH_API UFrontendUISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	static UFrontendUISubsystem* Get(const UObject* WorldContextObject);

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	UFUNCTION(BlueprintCallable)
	void RegisterCreatedPrimaryLayoutWidget(UWidget_PrimaryLayout* InWidget);

	void PushSoftWidgetToStackAsync(const FGameplayTag& WidgetTag, TSoftClassPtr<UWidget_ActivatableBase> InSoftWidgetClass, TFunction<void(EAsyncPushWidgetState, UWidget_ActivatableBase*)> AysncPushStateCallback);

private:
	UPROPERTY(Transient)
	TObjectPtr<UWidget_PrimaryLayout> CreatedPrimaryLayoutWidget;	// 创建的主布局小部件
};
