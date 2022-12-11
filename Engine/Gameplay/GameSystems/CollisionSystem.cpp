#include "CollisionSystem.h"

#include "Engine/Gameplay/GameplayUtiltiy.h"

TypeInfo const CollisionSystem::s_TypeInfo = TypeInfo("CollisionSystem", {});

void CollisionSystem::AddToLayer(NameHash layerName, CollisionComponent* collisionComponent)
{
	LayerType& layer = m_LayerMap[layerName];
	auto itr = std::find(layer.cbegin(), layer.cend(), collisionComponent);
	if (itr == layer.cend())
	{
		layer.push_back(ComponentWeakRef(collisionComponent));
	}
}

void CollisionSystem::RemoveFromLayer(NameHash layerName, CollisionComponent* collisionComponent)
{
	LayerType& layer = m_LayerMap[layerName];
	RemoveFromLayer(layer, collisionComponent);
}

void CollisionSystem::RemoveFromAllLayers(CollisionComponent* collisionComponent)
{
	for (auto& pair : m_LayerMap)
	{
		RemoveFromLayer(pair.second, collisionComponent);
	}
}

void CollisionSystem::RemoveFromLayer(LayerType& layer, CollisionComponent* collisionComponent)
{
	CoreUtility::RemoveSwapFirstMatchingItem(layer, collisionComponent);
}

void CollisionSystem::RemoveInvalidComponents()
{
	for (auto& pair : m_LayerMap)
	{
		LayerType& layer = pair.second;
		RemoveInvalidComponentsFromLayer(layer);
	}
}

void CollisionSystem::RemoveInvalidComponentsFromLayer(LayerType& layer)
{
	CoreUtility::RemoveSwapAllNullItems(layer);
}

void CollisionSystem::CheckAgainstLayer(CollisionComponent* collisionComponent, NameHash layerName,
	CallbackEvent<void,GameObject*,CollisionComponent*>& eventCollisionHandler)
{
	// Cache collisions, to avoid problem with objects being added during iteration.
	std::vector<CollisionComponent*> hitComponents;

	LayerType& layer = m_LayerMap[layerName];
	bool foundInvalid = false;
	for (WeakRef<CollisionComponent>& otherComponent : layer)
	{
		if (!otherComponent.IsValid())
		{
			foundInvalid = true;
			continue;
		}

		// Don't collide with self.
		if (otherComponent->GetOwner() == collisionComponent->GetOwner())
		{
			continue;
		}

		if (collisionComponent->CollidesWith(*otherComponent))
		{
			hitComponents.push_back(otherComponent.Get());
		}	
	}

	for (CollisionComponent* otherComponent : hitComponents)
	{
		eventCollisionHandler.Execute(otherComponent->GetOwner(), otherComponent);
	}

	if (foundInvalid)
	{
		RemoveInvalidComponentsFromLayer(layer);
	}
}
