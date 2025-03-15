// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ArcaneEnumTypes.h"

class FArcaneCountDownAction : public FPendingLatentAction
{
public:
	FArcaneCountDownAction(float InTotalCountDownTime, float InUpdateInterval, float& InOutRemainingTime, EArcaneCountDownActionOutput& InCountDownActionOutput, const FLatentActionInfo& LatentInfo)
		: bNeedToCancel(false)
		, TotalCountDownTime(InTotalCountDownTime)
		, UpdateInterval(InUpdateInterval)
		, OutRemainingTime(InOutRemainingTime)
		, CountDownActionOutput(InCountDownActionOutput)
		, ExecutionFunction(LatentInfo.ExecutionFunction)
		, OutputLink(LatentInfo.Linkage)
		, CallbackTarget(LatentInfo.CallbackTarget)
		, ElapsedIntervalTime(0.f)
		, ElapsedTimeSinceStart(0.f)
	{
	}

	void CancelAction();

private:
	bool bNeedToCancel;	// 是否需要取消
	float TotalCountDownTime;	// 总倒计时时间
	float UpdateInterval;		// 更新间隔
	
	float& OutRemainingTime;		// 剩余时间

	EArcaneCountDownActionOutput& CountDownActionOutput;
	FName ExecutionFunction;
	int32 OutputLink;
	FWeakObjectPtr CallbackTarget;
	float ElapsedIntervalTime;	// 已经过去的时间
	float ElapsedTimeSinceStart;	// 从开始到现在经过的时间
};
