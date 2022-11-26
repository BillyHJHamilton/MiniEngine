#include "Asteroid.h"

#include <cstdlib>
#include "Engine/Gameplay/Components/CollisionComponent.h"
#include "Engine/Gameplay/Components/MoveComponent.h"
#include "Engine/Gameplay/Components/SpriteComponent.h"
#include "Engine/Gameplay/Components/WrapAroundComponent.h"
#include "Engine/Math.h"

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
		float rotation = static_cast<float>(std::rand() % 360); // TODO better random library
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

	m_WrapAroundComponent = AddComponent<WrapAroundComponent>();
	if (m_WrapAroundComponent)
	{
		m_WrapAroundComponent->m_BoundSize = { 640.0f, 480.0f };
	}
}