#include "Asteroid.h"

#include "Asteroids/ScoreSystem.h"
#include "Engine/Gameplay/Components/CollisionComponent.h"
#include "Engine/Gameplay/Components/MoveComponent.h"
#include "Engine/Gameplay/Components/OutsideComponent.h"
#include "Engine/Gameplay/Components/SpriteComponent.h"
#include "Engine/Gameplay/World.h"
#include "Engine/Math.h"
#include "Engine/Random.h"

void Asteroid::Init()
{
	m_CollisionComponent = AddComponent<CollisionComponent>();
	if (m_CollisionComponent)
	{
		float colliderRadius = 26.0f * m_Scale;

		m_CollisionComponent->AddToLayer("Asteroid");
		m_CollisionComponent->SetCollider(new CircleCollider(colliderRadius));
		//m_CollisionComponent->m_DebugDraw = true;
	}

	m_MoveComponent = AddComponent<MoveComponent>();
	if (m_MoveComponent)
	{
		float rotation = Random::FloatInRange(0.0f, 360.0f);
		Vec2 unitVector = Math::RotationToUnitVector(rotation);
		m_MoveComponent->SetVelocity(m_FloatSpeed * unitVector);
	}

	m_SpriteComponent = EmplaceComponent<SpriteComponent>("Asteroid");
	if (m_SpriteComponent)
	{
		m_SpriteComponent->SetupSubimages({ 64,64 }, { 0,0 }, 50, 50, 0);
		m_SpriteComponent->m_Sprite.setOrigin(32, 32);
		m_SpriteComponent->m_Sprite.setScale({m_Scale,m_Scale});
		m_SpriteComponent->Animate(SpriteComponent::AnimationMode::SubimagesPerSecond, 20.0f, true);
	}

	m_OutsideComponent = AddComponent<OutsideComponent>();
	if (m_OutsideComponent)
	{
		m_OutsideComponent->SetResponse(OutsideComponent::Response::Wrap);
		m_OutsideComponent->SetReferenceSprite(m_SpriteComponent);
	}
}

void Asteroid::OnDestroy()
{
	ScoreSystem* scoreSystem = GetWorld()->GetSystem<ScoreSystem>();
	if (scoreSystem)
	{
		scoreSystem->AddScore(100);
	}
}
