// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ArcaneCharacterBase.h"
#include "GameplayTagContainer.h"
#include "ArcaneTypes/ArcaneEnumTypes.h"
#include "ArcaneTypes/ArcaneStructTypes.h"
#include "ArcaneHeroCharacter.generated.h"

class UHeroUIComponent;
struct FInputActionValue;
class UCameraComponent;
class USpringArmComponent;
class UDadaAsset_InputConfig;
class UHeroCombatComponent;

UCLASS()
class ARCANEGLYPH_API AArcaneHeroCharacter : public AArcaneCharacterBase
{
	GENERATED_BODY()

public:
	AArcaneHeroCharacter();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// ~ Begin APawn Interface
	virtual void PossessedBy(AController* NewController) override;
	// ~ End APawn Interface

	// IPawnCombatInterface
	virtual UPawnCombatComponent* GetPawnCombatComponent() const override;
	// ~ IPawnCombatInterface

	// IPawnUIInterface
	virtual UPawnUIComponent* GetPawnUIComponent() const override;
	virtual UHeroUIComponent* GetHeroUIComponent() const override;
	// ~ IPawnUIInterface

	FORCEINLINE UHeroCombatComponent* GetHeroCombatComponent() const { return HeroCombatComponent; }

	// 是否是左手正持有武器
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool bIsLeftHandUsingWeapon = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	TObjectPtr<AActor> SoftLockedTargetActor;		// 软锁定目标角色

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	TObjectPtr<AActor> CurrentLockedTargetActor;		// 当前锁定的目标角色

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilitySystem")
	float CentripetalFactor = 10000.f;		// 向心力因子

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ArcaneMovement")
	EArcaneGaits CurrentGait = EArcaneGaits::Walking;		// 当前步态

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ArcaneMovement")
	EArcaneGaits PreviousGait = EArcaneGaits::Walking;		// 上一个步态

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ArcaneMovement")
	TMap<EArcaneGaits, FArcaneGaitSetting> ArcaneGaits;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ArcaneMovement")
	double DoubleJumpZVelocity = 450.0f;		// 跳跃速度

	UFUNCTION(BlueprintCallable)
	void UpdateGait(EArcaneGaits InNewGait);

	UFUNCTION(BlueprintCallable, Category = "Arcane | Character")
	bool IsCanDoubleJump() const;

	UFUNCTION(BlueprintCallable, Category = "Arcane | Character", meta = (DisplayName = "Get Character Jump State", ExpandEnumAsExecs = "JumpState"))
	bool CharacterJumpState(EArcaneJumpStateType& JumpState);

	UFUNCTION(BlueprintCallable, Category = "Arcane | Character")
	void DoubleJump(float InDoubleJumpZVelocity);

	// 依据当前角色的速度和运动组件的最大跳跃高度计算跳跃结束位置
	UFUNCTION(BlueprintCallable, Category = "Arcane | Character")
	FVector GetCharacterJumpEndLocationByVelocity(bool bEnableDebug = false) const;

protected:
	virtual void BeginPlay() override;
	
	virtual void OnDeath() override;

private:
#pragma region Components

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UHeroCombatComponent* HeroCombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UHeroUIComponent* HeroUIComponent;
	
#pragma endregion

#pragma region Input

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDadaAsset_InputConfig> InputConfigDataAsset;

	void ProcessLockedMovement(float InputAxisValue);
	
	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);
	void Input_Jump();
	void Input_SwitchTargetTriggered(const FInputActionValue& InputActionValue);
	void Input_SwitchTargetCompleted(const FInputActionValue& InputActionValue);
	void Input_SwitchCurrentUseItem_Left(const FInputActionValue& InputActionValue);
	void Input_SwitchCurrentUseItem_Right(const FInputActionValue& InputActionValue);

	FVector2d SwitchDirection = FVector2d::ZeroVector;

	void Input_AbilityPressed(FGameplayTag InInputTag);
	void Input_AbilityReleased(FGameplayTag InInputTag);

#pragma endregion
	
};
