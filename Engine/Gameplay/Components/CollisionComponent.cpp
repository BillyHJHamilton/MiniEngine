#include "CollisionComponent.h"

#include "Engine/Gameplay/GameObject.h"
#include "Engine/Gameplay/GameSystems/CollisionSystem.h"
#include "Engine/Gameplay/World.h"
#include "Engine/Math.h"

TypeInfo const CollisionComponent::s_TypeInfo = TypeInfo("CollisionComponent", {"Component"});

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
	return (result2 == ECollisionResult::Collision);
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

void CollisionComponent::Draw(sf::RenderTarget& renderTarget, const sf::RenderStates& renderStates) const
{
	if (m_DebugDraw && m_Collider)
	{
		m_Collider->DebugDraw(renderTarget, renderStates);
	}
}

CollisionSystem* CollisionComponent::GetCollisionSystem()
{
	assert(GetOwner() && GetOwner()->GetWorld());
	return GetOwner()->GetWorld()->GetSystem<CollisionSystem>();
}
