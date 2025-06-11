// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonTabListWidgetBase.h"
#include "FrontendTabListWidgetBase.generated.h"

class UFrontendCommonButtonBase;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta=(DisabledNativeTick))
class ARCANEGLYPH_API UFrontendTabListWidgetBase : public UCommonTabListWidgetBase
{
	GENERATED_BODY()

public:
	void RequestRegisterTab(const FName& InTabID, const FText& InTabDisplayName);

private:
	// ~ Begin UWidget interface
#if WITH_EDITOR
	virtual void ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const override;
#endif
	// ~ End UWidget interface
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Frontend Tab List Widget", meta = (AllowPrivateAccess = "true", ClampMin = "1", ClampMax = "10"))
	int32 DebugEditorPreviewTabCount = 3;	// 用于调试编辑器预览标签的计数器

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Frontend Tab List Widget", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UFrontendCommonButtonBase> TabButtonEntryWidgetClass;	// 标签按钮条目的小部件类
};
