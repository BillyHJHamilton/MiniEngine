#pragma once

#include "Engine/Core.h"
#include "Engine/Gameplay/Component.h"
#include "Engine/Gameplay/GameObject.h"

template<typename DerivedComponent>
WeakRef<DerivedComponent> ComponentWeakRef(DerivedComponent* ptr)
{
	assert(ptr->GetOwner());
	return ptr->GetOwner()->MakeComponentWeakRef<DerivedComponent>(ptr);
}

template<typename DerivedComponent>
WeakRef<const DerivedComponent> ComponentWeakRef(const DerivedComponent* ptr)
{
	assert(ptr->GetOwner());
	return ptr->GetOwner()->MakeComponentWeakRef<const DerivedComponent>(ptr);
}
