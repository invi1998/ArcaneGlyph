// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "ArcaneCharacterBase.generated.h"

class UArcaneAttributeSet;
class UArcaneAbilitySystemComponent;

UCLASS()
class ARCANEGLYPH_API AArcaneCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AArcaneCharacterBase();

	FORCEINLINE UArcaneAbilitySystemComponent* GetArcaneAbilitySystemComponent() const { return ArcaneAbilitySystemComponent.Get(); }
	FORCEINLINE UArcaneAttributeSet* GetArcaneAttributeSet() const { return ArcaneAttributeSet.Get(); }

	// IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	// ~ IAbilitySystemInterface

protected:
	// ~ Begin APawn Interface
	virtual void PossessedBy(AController* NewController) override;	// 当角色被控制器控制时调用

	// ~ End APawn Interface
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Arcane | Ability System")
	TObjectPtr<UArcaneAbilitySystemComponent> ArcaneAbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Arcane | Ability System")
	TObjectPtr<UArcaneAttributeSet> ArcaneAttributeSet;

	
};
