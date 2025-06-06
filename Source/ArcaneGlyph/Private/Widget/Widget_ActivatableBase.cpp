// INVI_1998 All Rights Reserved.


#include "Widget/Widget_ActivatableBase.h"

#include "Controllers/ArcaneHeroController.h"

AArcaneHeroController* UWidget_ActivatableBase::GetOwningHeroController()
{
	if (!CachedOwningPC.IsValid())
	{
		CachedOwningPC = GetOwningPlayer<AArcaneHeroController>();
	}

	return CachedOwningPC.IsValid() ? CachedOwningPC.Get() : nullptr;
}
