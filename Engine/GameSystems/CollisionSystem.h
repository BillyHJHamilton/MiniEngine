#pragma once

#include "Engine/Core.h"

#include "Engine/ComponentReference.h"
#include "Engine/Components/CollisionComponent.h"
#include "Engine/GameSystem.h"

class CollisionSystem : public GameSystem
{
public:
	static NameHash StaticType() { return NameHash("CollisionSystem"); }
	virtual NameHash Type() { return GameSystem::StaticType(); }

	void AddToLayer(NameHash layerName, CollisionComponent* collisionComponent);
	void RemoveFromLayer(NameHash layerName, CollisionComponent* collisionComponent);
	void RemoveFromAllLayers(CollisionComponent* collisionComponent);
	void RemoveInvalidComponents();

	void CheckAgainstLayer(CollisionComponent* collisionComponent, NameHash layerName,
		CallbackEvent<void,GameObject*,CollisionComponent*>& eventCollisionHandler);

	//virtual void Tick(float deltaTime) override;

private:
	using LayerType = std::vector<ComponentReference<CollisionComponent>>;
	std::unordered_map<NameHash, LayerType> m_LayerMap;
};
