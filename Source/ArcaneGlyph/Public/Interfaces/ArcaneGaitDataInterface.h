// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ArcaneGaitDataInterface.generated.h"

enum class EArcaneGaits : uint8;
// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UArcaneGaitDataInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ARCANEGLYPH_API IArcaneGaitDataInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)	// 蓝图可调用，原生事件
	void ReceiveGaitData(const EArcaneGaits InGait);	// 接收步态数据
	
};
