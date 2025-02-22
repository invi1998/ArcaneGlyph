// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ArcaneWidgetBase.generated.h"

class UHeroUIComponent;
/**
 * 
 */
UCLASS()
class ARCANEGLYPH_API UArcaneWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:

protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Arcane | Widget", meta = (DisplayName = "On Owning Hero UI Component Initialized"))
	void BP_OnOwningHeroUIComponentInitialized(UHeroUIComponent* InHeroUIComponent);
};
