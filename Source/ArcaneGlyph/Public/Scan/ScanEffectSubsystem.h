// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "ScanEffectSubsystem.generated.h"

/**
 * 高级扫描效果子系统
 */


USTRUCT(BlueprintType)
struct FScanEffectConfig
{
	GENERATED_BODY()

	// 扫描颜色
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scan Effect")
	FLinearColor ScanColor = FLinearColor(0.0f, 1.0f, 1.0f, 1.0f);

	// 扫描线厚度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scan Effect")
	float ScanLineThickness = 2.0f;

	// 扫描速度（单位/秒）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scan Effect")
	float ScanSpeed = 200.0f;

	// 扫描持续时间
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scan Effect")
	float Duration = 3.0f;

	// 是否显示网格线框
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scan Effect")
	bool bShowWireframe = true;

	// 是否高亮显示
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scan Effect")
	bool bUseHighlight = true;

	// Niagara特效模板
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scan Effect")
	TSoftObjectPtr<UNiagaraSystem> NiagaraTemplate;
};

USTRUCT()
struct FActiveScanEffect
{
	GENERATED_BODY()

	UPROPERTY()
	TWeakObjectPtr<AActor> TargetActor;

	UPROPERTY()
	float ElapsedTime = 0.0f;

	UPROPERTY()
	float TotalDuration = 3.0f;

	UPROPERTY()
	UNiagaraComponent* NiagaraComponent = nullptr;

	UPROPERTY()
	TArray<UMeshComponent*> OriginalMeshes;

	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> DynamicMaterials;

	// 扫描方向（用于扫描线移动）
	UPROPERTY()
	FVector ScanDirection = FVector::UpVector;

	// 当前扫描位置
	UPROPERTY()
	float CurrentScanHeight = 0.0f;

	UPROPERTY()
	FScanEffectConfig Config;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnScanEffectFinished, AActor*, TargetActor, bool, bCompleted);

UCLASS()
class ARCANEGLYPH_API UScanEffectSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	// 子系统接口
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	// 核心功能：开始扫描效果
	UFUNCTION(BlueprintCallable, Category = "Scan Effect", meta = (DefaultToSelf = "TargetActor"))
	void StartScanEffect(AActor* TargetActor, const FScanEffectConfig& Config);

	// 核心功能：结束扫描效果
	UFUNCTION(BlueprintCallable, Category = "Scan Effect")
	void StopScanEffect(AActor* TargetActor);

	// 检查是否正在扫描
	UFUNCTION(BlueprintCallable, Category = "Scan Effect")
	bool IsScanning(AActor* TargetActor) const;

	// 获取正在扫描的Actor
	UFUNCTION(BlueprintCallable, Category = "Scan Effect")
	TArray<AActor*> GetCurrentlyScanningActors() const;

	// 预设配置
	UFUNCTION(BlueprintCallable, Category = "Scan Effect")
	void RegisterPresetConfig(FName PresetName, const FScanEffectConfig& Config);

	// 使用预设配置开始扫描
	UPROPERTY(BlueprintAssignable, Category = "Scan Effect")
	void StartScanWithPreset(AActor* TargetActor, FName PresetName);

	// 事件委托
	UPROPERTY(BlueprintAssignable, Category = "Scan Effect")
	FOnScanEffectFinished OnScanEffectFinished;

protected:
	// Tick更新扫描结果
	void UpdateScanEffect(float DeltaTime);

	// 应用扫描材质到网格
	void ApplyScanMaterial(FActiveScanEffect& ScanEffect);

	// 恢复原始材质
	void RestoreOriginalMaterial(FActiveScanEffect& ScanEffect);

	// 更新扫描线位置
	void UpdateScanLine(FActiveScanEffect& ScanEffect, float DeltaTime);

	// 创建Niagara特效
	UNiagaraComponent* SpawnNiagaraEffect(const FScanEffectConfig& Config, AActor* TargetActor);

	// 计算扫描边界
	FBox CalculateActorBounds(AActor* TargetActor) const;
	void LoadDefaultMaterials();
	UMaterialInterface* GetScanMaterial(bool bWireframe);

private:
	// 活跃的扫描效果列表
	UPROPERTY()
	TArray<FActiveScanEffect> ActiveScanEffects;
	
	// 预设配置映射
	UPROPERTY()
	TMap<FName, FScanEffectConfig> PresetConfigs;
	
	// 定时器句柄，用于定期更新扫描效果
	FTimerHandle ScanEffectUpdateTimerHandle;
	
	// 材质缓存（优化性能，避免重复创建动态材质实例）
	UPROPERTY()
	TMap<FName, UMaterialInterface*> CachedMaterials;
	
	
};
