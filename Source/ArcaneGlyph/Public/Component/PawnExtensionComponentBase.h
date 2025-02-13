// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PawnExtensionComponentBase.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ARCANEGLYPH_API UPawnExtensionComponentBase : public UActorComponent
{
	GENERATED_BODY()

protected:
	template<typename T>
	T* GetOwningPawn() const
	{
		// static_assert 用来检查模板参数是否符合要求，TPointerIsConvertibleFromTo 用来检查 T 是否是 APawn 的子类
		static_assert(TPointerIsConvertibleFromTo<T, APawn>::Value, "T must be a subclass of APawn");
		return CastChecked<T>(GetOwner());
	}

	APawn* GetOwningPawn() const
	{
		return GetOwningPawn<APawn>();
	}

	template<typename T>
	T* GetOwningController() const
	{
		// static_assert 用来检查模板参数是否符合要求，TPointerIsConvertibleFromTo 用来检查 T 是否是 AController 的子类
		static_assert(TPointerIsConvertibleFromTo<T, AController>::Value, "T must be a subclass of AController");
		return GetOwningPawn<APawn>()->GetController<T>();
	}

};
