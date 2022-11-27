#pragma once

#include "Engine/Core.h"

#include "Engine/Gameplay/Components/CollisionComponent.h"
#include "Engine/Gameplay/GameSystem.h"

class CollisionSystem : public GameSystem
{
public:
	MACRO_DeclareTypeInfo(CollisionSystem)

	void AddToLayer(NameHash layerName, CollisionComponent* collisionComponent);
	void RemoveFromLayer(NameHash layerName, CollisionComponent* collisionComponent);
	void RemoveFromAllLayers(CollisionComponent* collisionComponent);
	void RemoveInvalidComponents();

	void CheckAgainstLayer(CollisionComponent* collisionComponent, NameHash layerName,
		CallbackEvent<void,GameObject*,CollisionComponent*>& eventCollisionHandler);

private:
	using LayerType = std::vector<WeakRef<CollisionComponent>>;

	void RemoveFromLayer(LayerType& layer, CollisionComponent* collisionComponent);

	std::unordered_map<NameHash, LayerType> m_LayerMap;
};
