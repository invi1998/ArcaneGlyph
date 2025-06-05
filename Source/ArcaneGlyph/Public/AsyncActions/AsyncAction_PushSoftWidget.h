// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncAction_PushSoftWidget.generated.h"

class UWidget_ActivatableBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPushSoftWidgetDelegate, UWidget_ActivatableBase*, PushedWidget);

/**
 * 异步操作：推送小部件（软链接对象）到栈中
 */
UCLASS()
class ARCANEGLYPH_API UAsyncAction_PushSoftWidget : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", HidePin = "WorldContextObject", BlueprintInternalUseOnly = "true", DisplayName = "Push Soft Widget To Widget Stack Async"))
	static UAsyncAction_PushSoftWidget* PushSoftWidget(
		const UObject* WorldContextObject,
		APlayerController* PlayerController,
		TSoftClassPtr<UWidget_ActivatableBase> InSoftWidgetClass,
		UPARAM(meta = (Categories = "Frontend.WidgetStack")) FGameplayTag WidgetTag,
		bool bFocusOnNewPushedWidget = true);

	UPROPERTY(BlueprintAssignable)
	FOnPushSoftWidgetDelegate OnWidgetCreatedBeforePush;	// 在推送之前创建小部件的委托

	UPROPERTY(BlueprintAssignable)
	FOnPushSoftWidgetDelegate OnWidgetAfterPush;			// 推送之后创建小部件的委托
};
