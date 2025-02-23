// INVI_1998 All Rights Reserved.

#pragma once

namespace Debug
{
	static void Print(const FString& Msg, const FColor& Color = FColor::White, int32 Key = -1)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(Key, 5.f, Color, Msg);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *Msg);
		}
	}
}
