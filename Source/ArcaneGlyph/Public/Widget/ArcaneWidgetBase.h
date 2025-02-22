// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ArcaneWidgetBase.generated.h"

class UEnemyUIComponent;
class UHeroUIComponent;
/**
 * 
 */
UCLASS()
class ARCANEGLYPH_API UArcaneWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void InitEnemyCreatedWidget(AActor* InEnemyActor);

protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Arcane | Widget", meta = (DisplayName = "On Owning Hero UI Component Initialized"))
	void BP_OnOwningHeroUIComponentInitialized(UHeroUIComponent* InHeroUIComponent);

	UFUNCTION(BlueprintImplementableEvent, Category = "Arcane | Widget", meta = (DisplayName = "On Enemy Created Widget Initialized"))
	void BP_OnEnemyCreatedWidgetInitialized(UEnemyUIComponent* InEnemyUIComponent);
	
};
