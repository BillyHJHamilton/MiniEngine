#pragma once

#include "Engine/Core.h"
#include "Engine/Component.h"
#include "Engine/Components/Collider.h"

class CollisionSystem;

class CollisionComponent : public Component
{
public:
	static NameHash StaticType() { return NameHash("CollisionComponent"); }
	virtual NameHash Type() { return CollisionComponent::StaticType(); }

	virtual void OnAddedToObject(GameObject* newOwner) override;
	void OnOwnerPositionChanged(Vec2 newPosition);

	void SetCollider(ICollider* newCollider);

	void AddToLayer(NameHash layerName);

	template<typename ReceiverType>
	void SetCollisionEventForLayer(NameHash layerName, ReceiverType* receiver,
		void (ReceiverType::*memberFunction)(GameObject*,CollisionComponent*))
	{
		m_LayerEventMap[layerName].BindDelegate(receiver, memberFunction);
	}

	void ClearCollisionEventForLayer(NameHash layerName);

	bool CollidesWith(const CollisionComponent& other) const;

	virtual void Tick(float deltaTime);

private:
	CollisionSystem* GetCollisionSystem();
	std::unique_ptr<ICollider> m_Collider;
	std::unordered_map<NameHash,CallbackEvent<void,GameObject*,CollisionComponent*>> m_LayerEventMap;
};
