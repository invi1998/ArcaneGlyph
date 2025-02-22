// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interfaces/PawnCombatInterface.h"
#include "Interfaces/PawnUIInterface.h"
#include "ArcaneCharacterBase.generated.h"

class UDataAsset_StartupDadaBase;
class UArcaneAttributeSet;
class UArcaneAbilitySystemComponent;

UCLASS()
class ARCANEGLYPH_API AArcaneCharacterBase : public ACharacter, public IAbilitySystemInterface, public IPawnCombatInterface, public IPawnUIInterface
{
	GENERATED_BODY()

public:
	AArcaneCharacterBase();

	FORCEINLINE UArcaneAbilitySystemComponent* GetArcaneAbilitySystemComponent() const { return ArcaneAbilitySystemComponent.Get(); }
	FORCEINLINE UArcaneAttributeSet* GetArcaneAttributeSet() const { return ArcaneAttributeSet.Get(); }

	// IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	// ~ IAbilitySystemInterface

	// IPawnCombatInterface
	virtual UPawnCombatComponent* GetPawnCombatComponent() const override;
	// ~ IPawnCombatInterface

	// IPawnUIInterface
	virtual UPawnUIComponent* GetPawnUIComponent() const override;
	// ~ IPawnUIInterface

protected:
	// ~ Begin APawn Interface
	virtual void PossessedBy(AController* NewController) override;	// 当角色被控制器控制时调用

	// ~ End APawn Interface
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	TObjectPtr<UArcaneAbilitySystemComponent> ArcaneAbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	TObjectPtr<UArcaneAttributeSet> ArcaneAttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData")
	TSoftObjectPtr<UDataAsset_StartupDadaBase> CharacterStartupData;
	
};
