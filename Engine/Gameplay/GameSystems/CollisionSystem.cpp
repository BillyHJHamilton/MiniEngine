#include "CollisionSystem.h"

#include "Engine/Gameplay/ComponentUtiltiy.h"

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
	LayerType& layer = m_LayerMap[layerName];
	for (WeakRef<CollisionComponent>& otherComponent : layer)
	{
		if (!otherComponent.IsValid() || otherComponent->GetOwner() == collisionComponent->GetOwner())
		{
			continue;
		}

		if (collisionComponent->CollidesWith(*otherComponent))
		{
			eventCollisionHandler.Execute(otherComponent->GetOwner(), otherComponent.Get());
		}	
	}
}
