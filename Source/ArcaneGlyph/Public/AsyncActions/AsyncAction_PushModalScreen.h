// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ArcaneTypes/ArcaneEnumTypes.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncAction_PushModalScreen.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnModalScreenButtonClickedDelegate, EModalButtonType, ButtonType);

/**
 * 
 */
UCLASS()
class ARCANEGLYPH_API UAsyncAction_PushModalScreen : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", HidePin = "WorldContextObject", BlueprintInternalUseOnly = "true", DisplayName = "Show Comfirmation Screen"))
	static UAsyncAction_PushModalScreen* PushModalScreen(
		const UObject* WorldContextObject,
		APlayerController* PlayerController,
		EModalType ModalType,
		const FText& ModalTitle = FText::GetEmpty(),
		const FText& ModalSubtitle = FText::GetEmpty(),
		const FText& ModalMessage = FText::GetEmpty(),
		const FText& ModalDescription = FText::GetEmpty(),
		const FSlateBrush& ModalIcon = FSlateBrush());

	virtual void Activate() override;

	UPROPERTY(BlueprintAssignable)
	FOnModalScreenButtonClickedDelegate OnModalScreenButtonClicked;	// 模态屏幕按钮点击事件委托

private:
	TWeakObjectPtr<UWorld> CachedOwingWorld;							// 缓存的拥有世界的弱指针
	TWeakObjectPtr<APlayerController> CachedPlayerController;			// 缓存的玩家控制器的弱指针
	EModalType CachedModalType;											// 缓存的模态类型
	FText CachedModalTitle;											// 缓存的模态标题
	FText CachedModalSubtitle;										// 缓存的模态副标题
	FText CachedModalMessage;										// 缓存的模态消息
	FText CachedModalDescription;									// 缓存的模态描述
	FSlateBrush CachedModalIcon;										// 缓存的模态图标
	
};
