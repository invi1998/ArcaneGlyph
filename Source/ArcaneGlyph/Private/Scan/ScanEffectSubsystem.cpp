// INVI_1998 All Rights Reserved.


#include "Scan/ScanEffectSubsystem.h"
#include "NiagaraFunctionLibrary.h"

void UScanEffectSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	// 设置定时器，每帧调用一次 UpdateScanEffect 来更新扫描效果
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(
			ScanEffectUpdateTimerHandle,
			FTimerDelegate::CreateUObject(this, &UScanEffectSubsystem::UpdateScanEffect, 0.016f), // 约等于每帧60次
			0.016f,
			true
		);
	}
	
	// 预设配置示例
	// LoadDefaultMaterials();
	
}

void UScanEffectSubsystem::Deinitialize()
{
	// 停止所有活跃效果
	for (auto& Elem : ActiveScanEffects)
	{
		RestoreOriginalMaterial(Elem);
		if (Elem.NiagaraComponent)
		{
			Elem.NiagaraComponent->DestroyComponent();
		}
	}
	
	ActiveScanEffects.Empty();
	if (FTimerManager* TimerManager = GetWorld() ? &GetWorld()->GetTimerManager() : nullptr)
	{
		TimerManager->ClearTimer(ScanEffectUpdateTimerHandle);
	}
	
	Super::Deinitialize();
}

bool UScanEffectSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UScanEffectSubsystem::StartScanEffect(AActor* TargetActor, const FScanEffectConfig& Config)
{
	if (!IsValid(TargetActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("ScanEffectSubsystem: Invalid target actor"));
		return;
	}
	
	// 如果已经在扫描了，先停止当前的扫描效果
	StopScanEffect(TargetActor);
	
	// 创建新的扫描效果
	FActiveScanEffect NewScanEffect;
	NewScanEffect.TargetActor = TargetActor;
	NewScanEffect.Config = Config;
	NewScanEffect.TotalDuration = Config.Duration;
	NewScanEffect.ScanDirection = FVector::ForwardVector;
	
	// 计算初始扫描高度（从底部开始）
	FBox Bounds = CalculateActorBounds(TargetActor);
	NewScanEffect.CurrentScanHeight = Bounds.Min.Z;

	// 应用扫描材质
	ApplyScanMaterial(NewScanEffect);

	// 生成Niagara特效
	if (Config.NiagaraTemplate.IsValid())
	{
		NewScanEffect.NiagaraComponent = SpawnNiagaraEffect(Config, TargetActor);
	}

	ActiveScanEffects.Add(NewScanEffect);

	UE_LOG(LogTemp, Log, TEXT("Started scan effect on %s"), *TargetActor->GetName());
		
	
}

void UScanEffectSubsystem::StopScanEffect(AActor* TargetActor)
{
	if (!IsValid(TargetActor))
	{
		return;
	}
	
	for (int32 i = ActiveScanEffects.Num() - 1; i >= 0; --i)
	{
		if (ActiveScanEffects[i].TargetActor == TargetActor)
		{
			RestoreOriginalMaterial(ActiveScanEffects[i]);
			if (ActiveScanEffects[i].NiagaraComponent)
			{
				ActiveScanEffects[i].NiagaraComponent->DestroyComponent();
			}
			OnScanEffectFinished.Broadcast(TargetActor, false);
			ActiveScanEffects.RemoveAt(i);
			break;
		}
	}
	
}

bool UScanEffectSubsystem::IsScanning(AActor* TargetActor) const
{
	if (!IsValid(TargetActor)) return false;
	for (const FActiveScanEffect& ScanEffect : ActiveScanEffects)
	{
		if (ScanEffect.TargetActor.Get() == TargetActor)
		{
			return true;
		}
	}
	return false;
}

TArray<AActor*> UScanEffectSubsystem::GetCurrentlyScanningActors() const
{
	TArray<AActor*> Result;
	for (const FActiveScanEffect& ScanEffect : ActiveScanEffects)
	{
		if (ScanEffect.TargetActor.IsValid())
		{
			Result.Add(ScanEffect.TargetActor.Get());
		}
	}
	return Result;
}

void UScanEffectSubsystem::RegisterPresetConfig(FName PresetName, const FScanEffectConfig& Config)
{
	PresetConfigs.Add(PresetName, Config);
}

void UScanEffectSubsystem::StartScanWithPreset(AActor* TargetActor, FName PresetName)
{
	if (const FScanEffectConfig* Config = PresetConfigs.Find(PresetName))
	{
		StartScanEffect(TargetActor, *Config);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ScanEffectSubsystem: Preset %s not found"), *PresetName.ToString());
	}
}

void UScanEffectSubsystem::UpdateScanEffect(float DeltaTime)
{
	UWorld* World = GetWorld();
	if (!World) return;
	
	for (int32 i = ActiveScanEffects.Num() - 1; i >= 0; --i)
	{
		FActiveScanEffect& ScanEffect = ActiveScanEffects[i];
		
		if (!ScanEffect.TargetActor.IsValid())
		{
			// 目标无效，移除效果
			RestoreOriginalMaterial(ScanEffect);
			if (ScanEffect.NiagaraComponent)
			{
				ScanEffect.NiagaraComponent->DestroyComponent();
			}
			OnScanEffectFinished.Broadcast(ScanEffect.TargetActor.Get(), false);
			ActiveScanEffects.RemoveAt(i);
			continue;
		}
		
		ScanEffect.ElapsedTime += DeltaTime;
		UpdateScanLine(ScanEffect, DeltaTime);
		
		// 更新材质参数
		for (UMaterialInstanceDynamic* MID : ScanEffect.DynamicMaterials)
		{
			if (MID)
			{
				MID->SetScalarParameterValue(TEXT("ScanHeight"), ScanEffect.CurrentScanHeight);
				MID->SetScalarParameterValue(TEXT("ScanProgress"), ScanEffect.ElapsedTime / ScanEffect.TotalDuration);
				MID->SetVectorParameterValue(TEXT("ScanColor"), ScanEffect.Config.ScanColor);
			}
		}
		
		if (ScanEffect.ElapsedTime >= ScanEffect.TotalDuration)
		{
			// 扫描完成，移除效果
			RestoreOriginalMaterial(ScanEffect);
			if (ScanEffect.NiagaraComponent)
			{
				ScanEffect.NiagaraComponent->DestroyComponent();
			}
			OnScanEffectFinished.Broadcast(ScanEffect.TargetActor.Get(), true);
			ActiveScanEffects.RemoveAt(i);
		}
	}
	
}

void UScanEffectSubsystem::ApplyScanMaterial(FActiveScanEffect& ScanEffect)
{
	AActor* TargetActor = ScanEffect.TargetActor.Get();
	if (!IsValid(TargetActor)) return;
	
	// 获取场景中所有的Mesh组件
	TArray<UMeshComponent*> MeshComponents;
	TargetActor->GetComponents<UMeshComponent>(MeshComponents);
	
	for (UMeshComponent* MeshComp : MeshComponents)
	{
		if (IsValid(MeshComp))
		{
			// 保存原始材质
			TArray<UMaterialInterface*> OriginalMats;
			for (int32 MatIndex = 0; MatIndex < MeshComp->GetNumMaterials(); ++MatIndex)
			{
				OriginalMats.Add(MeshComp->GetMaterial(MatIndex));
			}
			ScanEffect.OriginalMeshes.Add(MeshComp);

			// 创建并应用扫描材质
			for (int32 MatIndex = 0; MatIndex < OriginalMats.Num(); ++MatIndex)
			{
				UMaterialInterface* OriginalMat = OriginalMats[MatIndex];
				if (!OriginalMat) continue;

				// 创建动态材质实例
				UMaterialInstanceDynamic* DynMat = UMaterialInstanceDynamic::Create(
					GetScanMaterial(ScanEffect.Config.bShowWireframe),
					this
				);

				// 设置基础纹理（从原始材质获取）
				FName BaseColorParam = "BaseTexture";
				// 这里可以复制原始材质的纹理参数

				// 设置扫描参数
				DynMat->SetVectorParameterValue("ScanColor", ScanEffect.Config.ScanColor);
				DynMat->SetScalarParameterValue("ScanLineThickness", ScanEffect.Config.ScanLineThickness);
				DynMat->SetScalarParameterValue("ScanHeight", ScanEffect.CurrentScanHeight);
				DynMat->SetScalarParameterValue("EmissionStrength", 2.0f);

				MeshComp->SetMaterial(MatIndex, DynMat);
				ScanEffect.DynamicMaterials.Add(DynMat);
			}

			// 启用Custom Depth用于后期处理效果（可选）
			MeshComp->SetRenderCustomDepth(true);
			MeshComp->SetCustomDepthStencilValue(1);
		}
	}
	
}

void UScanEffectSubsystem::RestoreOriginalMaterial(FActiveScanEffect& ScanEffect)
{
	int32 MatIndex = 0;
	for (UMeshComponent* MeshComp : ScanEffect.OriginalMeshes)
	{
		if (!IsValid(MeshComp)) continue;

		// 这里需要重新获取原始材质并恢复
		// 简化处理：在实际实现中应该存储原始材质的引用
        
		// 禁用Custom Depth
		MeshComp->SetRenderCustomDepth(false);
	}

	// 清理动态材质
	ScanEffect.DynamicMaterials.Empty();
}

void UScanEffectSubsystem::UpdateScanLine(FActiveScanEffect& ScanEffect, float DeltaTime)
{
	FBox Bounds = CalculateActorBounds(ScanEffect.TargetActor.Get());
	float HeightRange = Bounds.GetSize().Z;
    
	// 根据速度更新扫描高度
	ScanEffect.CurrentScanHeight += ScanEffect.Config.ScanSpeed * DeltaTime;

	// 循环扫描（可选）
	if (ScanEffect.CurrentScanHeight > Bounds.Max.Z)
	{
		ScanEffect.CurrentScanHeight = Bounds.Min.Z;
	}
}

UNiagaraComponent* UScanEffectSubsystem::SpawnNiagaraEffect(const FScanEffectConfig& Config, AActor* TargetActor)
{
	if (!Config.NiagaraTemplate.IsValid()) return nullptr;

	UNiagaraSystem* NiagaraSystem = Config.NiagaraTemplate.LoadSynchronous();
	if (!NiagaraSystem) return nullptr;

	FBox Bounds = CalculateActorBounds(TargetActor);
	FVector Location = Bounds.GetCenter();

	UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		NiagaraSystem,
		Location,
		FRotator::ZeroRotator,
		FVector(1.0f),
		true,   // bAutoDestroy
		true,   // bAutoActivate
		ENCPoolMethod::None,
		true    // bPreCullCheck
	);

	if (NiagaraComp)
	{
		// 设置Niagara参数
		NiagaraComp->SetColorParameter("ScanColor", Config.ScanColor);
		NiagaraComp->SetFloatParameter("ScanDuration", Config.Duration);
	}

	return NiagaraComp;
}

FBox UScanEffectSubsystem::CalculateActorBounds(AActor* TargetActor) const
{
	if (!IsValid(TargetActor)) return FBox(ForceInit);

	FVector Origin, Extent;
	TargetActor->GetActorBounds(false, Origin, Extent);
	return FBox(Origin - Extent, Origin + Extent);
}

void UScanEffectSubsystem::LoadDefaultMaterials()
{
	// 预加载扫描材质，避免运行时加载延迟
	// 路径需要根据实际项目调整
	ConstructorHelpers::FObjectFinder<UMaterial> WireframeMat(
		TEXT("/Game/Materials/M_ScanEffect_Wireframe.M_ScanEffect_Wireframe")
	);
    
	if (WireframeMat.Succeeded())
	{
		CachedMaterials.Add("Wireframe", WireframeMat.Object);
	}
}


UMaterialInterface* UScanEffectSubsystem::GetScanMaterial(bool bWireframe)
{
	FName Key = bWireframe ? "Wireframe" : "Solid";
	if (UMaterialInterface** Found = CachedMaterials.Find(Key))
	{
		return *Found;
	}
    
	// 返回默认材质（应该在Content Browser中创建）
	return nullptr;
}
