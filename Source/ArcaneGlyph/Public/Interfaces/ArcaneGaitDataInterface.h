// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ArcaneGaitDataInterface.generated.h"

enum class EArcaneGaits : uint8;
// This class does not need to be modified.
UINTERFACE()
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
	virtual bool ReceiveGaitData(const EArcaneGaits InGait) = 0;	// 接收步态数据
	
};
