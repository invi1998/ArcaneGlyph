﻿// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PawnUIInterface.generated.h"

class UEnemyUIComponent;
class UHeroUIComponent;
class UPawnUIComponent;
// This class does not need to be modified.
UINTERFACE()
class UPawnUIInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ARCANEGLYPH_API IPawnUIInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual UPawnUIComponent* GetPawnUIComponent() const = 0;
	virtual UHeroUIComponent* GetHeroUIComponent() const;
	virtual UEnemyUIComponent* GetEnemyUIComponent() const;
	
};
