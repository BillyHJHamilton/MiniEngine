#include "Laser.h"

#include "Engine/Gameplay/Components/CollisionComponent.h"
#include "Engine/Gameplay/Components/MoveComponent.h"
#include "Engine/Gameplay/Components/OutsideComponent.h"
#include "Engine/Gameplay/Components/SpriteComponent.h"
#include "Engine/Gameplay/World.h"
#include "Engine/Gameplay/GameSystems/TimerSystem.h"
#include "Engine/Math.h"
#include "Explosion.h"

void Laser::Init()
{
	m_CollisionComponent = AddComponent<CollisionComponent>();
	if (m_CollisionComponent)
	{
		m_CollisionComponent->SetCollider(new PointCollider());
		m_CollisionComponent->SetCollisionEventForLayer("Asteroid", this, &Laser::OnHitAsteroid);
	}

	m_MoveComponent = AddComponent<MoveComponent>();

	m_SpriteComponent = EmplaceComponent<SpriteComponent>("BlueLaser");
	if (m_SpriteComponent)
	{
		m_SpriteComponent->m_Sprite.setOrigin(2, 2);
	}

	m_OutsideComponent = AddComponent<OutsideComponent>();
	if (m_OutsideComponent)
	{
		m_OutsideComponent->SetResponse(OutsideComponent::Response::Destroy);
		m_OutsideComponent->SetReferenceSprite(m_SpriteComponent);
	}
}

void Laser::Shoot(float sfRotation, float speed)
{
	if (m_SpriteComponent)
	{
		m_SpriteComponent->m_Sprite.setRotation(sfRotation);
	}

	if (m_MoveComponent)
	{
		Vec2 unit = Math::RotationToUnitVector(sfRotation);
		m_MoveComponent->SetVelocity(speed * unit);
	}
}

void Laser::OnHitAsteroid(GameObject* otherObject, CollisionComponent* otherComponent)
{
	assert(GetWorld());
	ExplodeObject(*this);
	otherObject->Destroy();
}
