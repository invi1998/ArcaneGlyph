// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interfaces/PawnCombatInterface.h"
#include "Interfaces/PawnUIInterface.h"
#include "ArcaneCharacterBase.generated.h"

class UBoxComponent;
class UDataAsset_StartupDadaBase;
class UArcaneAttributeSet;
class UArcaneAbilitySystemComponent;
class UMotionWarpingComponent;

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

	FORCEINLINE UBoxComponent* GetLeftHandCollisionBox() const { return LeftHandCollisionBox; }
	FORCEINLINE UBoxComponent* GetRightHandCollisionBox() const { return RightHandCollisionBox; }
	FORCEINLINE UBoxComponent* GetHeadCollisionBox() const { return HeadCollisionBox; }
	FORCEINLINE UBoxComponent* GetLeftFootCollision() const { return LeftFootCollisionBox; }
	FORCEINLINE UBoxComponent* GetRightFootCollision() const { return RightFootCollisionBox; }

	UFUNCTION(BlueprintCallable, Category = "Arcane | Character")
	bool IsCharacterAlive() const;

protected:
	// ~ Begin APawn Interface
	virtual void PossessedBy(AController* NewController) override;	// 当角色被控制器控制时调用

	// ~ End APawn Interface

#if WITH_EDITOR
	// Begin UObject Interface
	// 
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	// End UObject Interface
#endif
	
	UFUNCTION()
	virtual void OnBodyCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	// TObjectPtr<UMotionWarpingComponent> CharacterMotionWarpingComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UBoxComponent> LeftHandCollisionBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	FName LeftHandCollisionBoxAttachBoneName;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UBoxComponent> RightHandCollisionBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	FName RightHandCollisionBoxAttachBoneName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UBoxComponent> HeadCollisionBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	FName HeadCollisionBoxAttachBoneName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UBoxComponent> LeftFootCollisionBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	FName LeftFootCollisionBoxAttachBoneName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UBoxComponent> RightFootCollisionBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	FName RightFootCollisionBoxAttachBoneName;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	TObjectPtr<UArcaneAbilitySystemComponent> ArcaneAbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	TObjectPtr<UArcaneAttributeSet> ArcaneAttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData")
	TSoftObjectPtr<UDataAsset_StartupDadaBase> CharacterStartupData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MotionWarping")
	TObjectPtr<UMotionWarpingComponent> MotionWarpingComponent;
	
};
