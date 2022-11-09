#pragma once

#include "Engine/Core.h"
#include "Engine/Gameplay/Component.h"
#include "Engine/Gameplay/Components/Collider.h"

class CollisionSystem;

class CollisionComponent : public Component
{
public:
	MACRO_DeclareTypeInfo(CollisionComponent)

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
	virtual void Draw(sf::RenderTarget& renderTarget, const sf::RenderStates& renderStates) const override;

	bool m_DebugDraw = false;

private:
	CollisionSystem* GetCollisionSystem();
	std::unique_ptr<ICollider> m_Collider;
	std::unordered_map<NameHash,CallbackEvent<void,GameObject*,CollisionComponent*>> m_LayerEventMap;
};
