#pragma once

#include "Engine/Core.h"
#include "Engine/Gameplay/Component.h"
#include "Engine/Gameplay/GameObject.h"
#include "Engine/Gameplay/World.h"

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

template<typename DerivedGameObject>
WeakRef<DerivedGameObject> ObjectWeakRef(DerivedGameObject* ptr)
{
	return ptr->MakeDerivedWeakRef<DerivedGameObject>();
}

template<typename DerivedGameObject>
WeakRef<const DerivedGameObject> ObjectWeakRef(const DerivedGameObject* ptr)
{
	return ptr->MakeDerivedWeakRef<const DerivedGameObject>();
}

inline bool IsValid(const Component* component)
{
	return component != nullptr
		&& component->GetOwner() != nullptr
		&& component->GetOwner()->IsValid();
}

template<typename ObjectType>
bool IsValid(const WeakRef<ObjectType>& ref)
{
	return ref.IsValid() && IsValid(ref.Get());
}

template<typename ObjectType>
ObjectType* SpawnObject(World& world, Vec2 position)
{
	ObjectType* newObject = world.AddObject<ObjectType>();
	if (newObject)
	{
		newObject->SetPosition(position);
	}
	return newObject;
}

template<typename ObjectType>
ObjectType* SpawnObjectAtObject(GameObject& existingObject)
{
	World* world = existingObject.GetWorld();
	ObjectType* newObject = world->AddObject<ObjectType>();
	if (newObject)
	{
		newObject->SetPosition(existingObject.GetPosition());
	}
	return newObject;
}
