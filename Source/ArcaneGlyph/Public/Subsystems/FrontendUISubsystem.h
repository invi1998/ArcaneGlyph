// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ArcaneTypes/ArcaneEnumTypes.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "FrontendUISubsystem.generated.h"

class UFrontendCommonButtonBase;
class UWidget_ActivatableBase;
struct FGameplayTag;
class UWidget_PrimaryLayout;

enum class EAsyncPushWidgetState : uint8
{
	OnCreatedBeforePush UMETA(DisplayName = "在创建推送之前"),
	AfterPush UMETA(DisplayName = "推送之后"),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnButtonDescriptionTextUpdateDelegate, UFrontendCommonButtonBase*, Button, FText, DescriptionText);

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

	void PushSoftWidgetToStackAsync(const FGameplayTag& WidgetTag, TSoftClassPtr<UWidget_ActivatableBase> InSoftWidgetClass, TFunction<void(EAsyncPushWidgetState, UWidget_ActivatableBase*)> AsyncPushStateCallback);

	void PushModalScreenToModalStackAsync(const FGameplayTag& WidgetTag, EModalType ModalType, const FText& ModalTitle, const FText& ModalSubtitle, const FText& ModalMessage, const FText& ModalDescription, const FSlateBrush& ModalIcon, TFunction<void(EModalButtonType)> ButtonClickedCallback);
	
	UPROPERTY(BlueprintAssignable)
	FOnButtonDescriptionTextUpdateDelegate OnButtonDescriptionTextUpdate;	// 按钮描述文本更新委托
	
private:
	UPROPERTY(Transient)
	TObjectPtr<UWidget_PrimaryLayout> CreatedPrimaryLayoutWidget;	// 创建的主布局小部件
};
