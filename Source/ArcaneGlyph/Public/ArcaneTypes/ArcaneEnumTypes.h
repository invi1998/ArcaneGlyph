// INVI_1998 All Rights Reserved.

#pragma once

UENUM()
enum class EArcaneConfirmType : uint8
{
	YES,
	NO
};

UENUM()
enum class EArcaneValidType : uint8
{
	Valid,		// 有效
	InValid		// 无效
};

UENUM()
enum class EArcaneSuccessType : uint8
{
	Success,
	Fail
};

UENUM(BlueprintType)
enum class EArcaneMoveDirection : uint8
{
	None        UMETA(DisplayName = "无"),
	Forward     UMETA(DisplayName = "前"),
	ForwardRight UMETA(DisplayName = "右前"),
	Right       UMETA(DisplayName = "右"),
	BackwardRight UMETA(DisplayName = "右后"),
	Backward    UMETA(DisplayName = "后"),
	BackwardLeft UMETA(DisplayName = "左后"),
	Left        UMETA(DisplayName = "左"),
	ForwardLeft UMETA(DisplayName = "左前")
};

UENUM(BlueprintType)
enum class EArcaneMovementDirectionMethod : uint8
{
	// 基于轴向分量绝对值的简单比较（4方向判定）
	// - 适用场景：快速判断主要移动方向（前后左右）
	// - 特点：计算简单，性能高效
	AxisDominant,
    
	// 基于角度阈值分割的精确方向判定（4方向）
	// - 适用场景：需要精确角度对齐的判定（如45°倾斜不视为斜向）
	// - 特点：通过扇形区域划分方向
	AngleThreshold4,
    
	// 基于45°划分的8方向判定
	// - 适用场景：需要支持斜向移动的复杂系统（如格斗游戏八向移动）
	// - 特点：更精细的方向区分
	AngleThreshold8,
    
	// 自定义扩展占位符
	Custom
	
};

UENUM()
enum class EArcaneCountDownActionInput : uint8
{
	// 开始倒计时
	Start,
	// 倒计时取消
	Cancel
};

UENUM()
enum class EArcaneCountDownActionOutput : uint8
{
	Update,
	Completed,
	Cancelled
};

UENUM(BlueprintType)
enum class EArcaneGaits : uint8
{
	Jogging UMETA(DisplayName = "慢跑"),
	Walking UMETA(DisplayName = "行走"),
	Running UMETA(DisplayName = "奔跑"),
};

UENUM(BlueprintType)
enum class EArcaneHipFacing : uint8
{
	Forward UMETA(DisplayName = "前"),
	Backward UMETA(DisplayName = "后")
};

UENUM(BlueprintType)
enum class EArcaneLocomotionDirection : uint8
{
	Forward UMETA(DisplayName = "前"),
	Backward UMETA(DisplayName = "后"),
	Left UMETA(DisplayName = "左"),
	Right UMETA(DisplayName = "右"),
};

UENUM(BlueprintType)
enum class EArcaneRootYawOffsetMode : uint8
{
	Accumulate UMETA(DisplayName = "累积"),
	BlendOut UMETA(DisplayName = "混合"),
	Hold UMETA(DisplayName = "保持"),
};


