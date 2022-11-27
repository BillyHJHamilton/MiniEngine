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
	auto itr = std::find(layer.begin(), layer.end(), collisionComponent);
	if (itr != layer.end())
	{
		*itr = std::move(layer.back());
		layer.pop_back();
	}
}

void CollisionSystem::RemoveInvalidComponents()
{
	for (auto& pair : m_LayerMap)
	{
		LayerType& layer = pair.second;
		for (int i = 0; i < layer.size(); ++i)
		{
			if (!layer[i].IsValid())
			{
				layer[i] = std::move(layer.back());
				layer.pop_back();
				--i;
			}
		}
	}
}

void CollisionSystem::CheckAgainstLayer(CollisionComponent* collisionComponent, NameHash layerName,
	CallbackEvent<void,GameObject*,CollisionComponent*>& eventCollisionHandler)
{
	// Cache collisions, to avoid problem with objects being added during iteration.
	// TODO maybe instead defer object spawning
	std::vector<CollisionComponent*> hitComponents;

	LayerType& layer = m_LayerMap[layerName];
	for (WeakRef<CollisionComponent>& otherComponent : layer)
	{
		if (!IsValid(otherComponent) || otherComponent->GetOwner() == collisionComponent->GetOwner())
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
}
