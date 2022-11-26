#include "CollisionComponent.h"

#include "Engine/GameObject.h"
#include "Engine/GameSystems/CollisionSystem.h"
#include "Engine/World.h"
#include "Engine/Math.h"

void CollisionComponent::OnAddedToObject(GameObject* newOwner)
{
	newOwner->EventPositionChanged.AddDelegate(this, &CollisionComponent::OnOwnerPositionChanged);
}

void CollisionComponent::OnOwnerPositionChanged(Vec2 newPosition)
{
	if (m_Collider)
	{
		m_Collider->m_Position = newPosition;
	}
}

void CollisionComponent::SetCollider(ICollider* newCollider)
{
	m_Collider = std::unique_ptr<ICollider>(newCollider);
	newCollider->m_Position = GetOwner()->GetPosition();
}

void CollisionComponent::AddToLayer(NameHash layerName)
{
	CollisionSystem* collisionSystem = GetCollisionSystem();
	if (collisionSystem)
	{
		collisionSystem->AddToLayer(layerName, this);
	}
}

void CollisionComponent::ClearCollisionEventForLayer(NameHash layerName)
{
	m_LayerEventMap.erase(layerName);
}

bool CollisionComponent::CollidesWith(const CollisionComponent& other) const
{
	if (!m_Collider || !other.m_Collider)
	{
		return false;
	}

	ECollisionResult const result1 = m_Collider->ResolveCollision(other.m_Collider.get());
	if (result1 != ECollisionResult::Unresolved)
	{
		return (result1 == ECollisionResult::Collision);
	}

	ECollisionResult const result2 = other.m_Collider->ResolveCollision(m_Collider.get());
	return (result1 == ECollisionResult::Collision);
}

void CollisionComponent::Tick(float deltaTime)
{
	CollisionSystem* collisionSystem = GetCollisionSystem();
	if (collisionSystem)
	{
		for (auto& pair : m_LayerEventMap)
		{
			NameHash layerName = pair.first;
			CallbackEvent<void,GameObject*,CollisionComponent*>& layerEvent = pair.second;
			collisionSystem->CheckAgainstLayer(this, layerName, layerEvent);
		}
	}
}

CollisionSystem* CollisionComponent::GetCollisionSystem()
{
	assert(GetOwner() && GetOwner()->GetWorld());
	return GetOwner()->GetWorld()->GetSystem<CollisionSystem>();
}
