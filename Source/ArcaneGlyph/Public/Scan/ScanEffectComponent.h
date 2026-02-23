// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ScanEffectSubsystem.h"
#include "Components/ActorComponent.h"
#include "ScanEffectComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ARCANEGLYPH_API UScanEffectComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UScanEffectComponent();
	
	// 开始扫描（使用默认配置）
	UFUNCTION(BlueprintCallable, Category = "Scan Effect")
	void StartScan();

	// 开始扫描（使用指定配置）
	UFUNCTION(BlueprintCallable, Category = "Scan Effect")
	void StartScanWithConfig(const FScanEffectConfig& Config);

	// 开始扫描（使用预设）
	UFUNCTION(BlueprintCallable, Category = "Scan Effect")
	void StartScanWithPreset(FName PresetName);

	// 停止扫描
	UFUNCTION(BlueprintCallable, Category = "Scan Effect")
	void StopScan();

	// 是否正在扫描
	UFUNCTION(BlueprintPure, Category = "Scan Effect")
	bool IsScanning() const;

	// 组件默认配置
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scan Effect")
	FScanEffectConfig DefaultConfig;

	// 自动开始扫描（用于测试）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scan Effect")
	bool bAutoStartOnBeginPlay = false;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
private:
	UPROPERTY()
	UScanEffectSubsystem* ScanSubsystem;

	void InitializeSubsystem();
	
	
};
