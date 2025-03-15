// INVI_1998 All Rights Reserved.


#include "ArcaneTypes/ArcaneCountDownAction.h"

void FArcaneCountDownAction::UpdateOperation(FLatentResponse& Response)
{
	if (bNeedToCancel)
	{
		// 如果需要结束，那么就需要将输出引脚Cancel触发为开启状态
		CountDownActionOutput = EArcaneCountDownActionOutput::Cancelled;

		Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallbackTarget);

		return;
	}

	if (ElapsedTimeSinceStart >= TotalCountDownTime)
	{
		// 如果计时时间已经大于总时间，视为活动完成
		CountDownActionOutput = EArcaneCountDownActionOutput::Completed;

		Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallbackTarget);

		return;
	}

	if (ElapsedIntervalTime < UpdateInterval)
	{
		// 如果当前时间小于更新间隔时间，那么就将时间累加
		ElapsedIntervalTime += Response.ElapsedTime();
	}
	else
	{
		// 在累加倒计时时间的时候，需要判断更新间隔是否大于0，如果没有，则说明该间隔程序未设置，就需要使用Response.ElapsedTime()来累加
		// 同时，如果更新间隔为0，表示用户希望每帧都更新，那么就需要使用Response.ElapsedTime()来累加
		ElapsedTimeSinceStart += UpdateInterval > 0.f ? UpdateInterval : Response.ElapsedTime();

		// 计算剩余时间
		OutRemainingTime = TotalCountDownTime - ElapsedTimeSinceStart;

		// 然后激活更新输出引脚，执行Tick。然后将累加的时间清零
		CountDownActionOutput = EArcaneCountDownActionOutput::Update;
		Response.TriggerLink(ExecutionFunction, OutputLink, CallbackTarget);
		ElapsedIntervalTime = 0.f;
	}
}

void FArcaneCountDownAction::CancelAction()
{
	bNeedToCancel = true;
}
