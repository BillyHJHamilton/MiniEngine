#include "CollisionSystem.h"

void CollisionSystem::AddToLayer(NameHash layerName, CollisionComponent* collisionComponent)
{
	LayerType& layer = m_LayerMap[layerName];
	auto itr = std::find(layer.cbegin(), layer.cend(), collisionComponent);
	if (itr == layer.cend())
	{
		layer.push_back( ComponentReference<CollisionComponent>(collisionComponent) );
	}
}

void CollisionSystem::RemoveFromLayer(NameHash layerName, CollisionComponent* collisionComponent)
{
	LayerType& layer = m_LayerMap[layerName];
	auto itr = std::find(layer.begin(), layer.end(), collisionComponent);
	if (itr != layer.end())
	{
		*itr = layer.back();
		layer.pop_back();
	}
}

void CollisionSystem::RemoveFromAllLayers(CollisionComponent* collisionComponent)
{
	for (auto& pair : m_LayerMap)
	{
		LayerType& layer = pair.second;
		auto itr = std::find(layer.begin(), layer.end(), collisionComponent);
		if (itr != layer.end())
		{
			*itr = layer.back();
			layer.pop_back();
		}
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
				layer[i] = layer.back();
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
	for (ComponentReference<CollisionComponent>& otherComponent : layer)
	{
		if (!otherComponent.IsValid() || otherComponent == collisionComponent)
		{
			continue;
		}

		if (collisionComponent->CollidesWith(*otherComponent)) // TODO should be a collision check
		{
			eventCollisionHandler.Execute(otherComponent->GetOwner(), otherComponent.Get());
		}	
	}
}
