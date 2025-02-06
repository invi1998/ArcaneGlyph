// INVI_1998 All Rights Reserved.


#include "Characters/ArcaneCharacterBase.h"

AArcaneCharacterBase::AArcaneCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	GetMesh()->bReceivesDecals = false;		// 角色模型不接受贴花
}




