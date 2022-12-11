#include "PlayerShip.h"

#include "Asteroids/Explosion.h"
#include "Asteroids/Laser.h"
#include "Engine/App/GameApp.h"
#include "Engine/Gameplay.h"
#include "Engine/Gameplay/Components/CollisionComponent.h"
#include "Engine/Gameplay/Components/MoveComponent.h"
#include "Engine/Gameplay/Components/SpriteComponent.h"
#include "Engine/Gameplay/Components/OutsideComponent.h"
#include "Engine/Gameplay/GameSystems/TimerSystem.h"
#include "Engine/Math.h"
#include "SFML/Window/Keyboard.hpp"

void PlayerShip::Init()
{
	m_CollisionComponent = AddComponent<CollisionComponent>();
	if (m_CollisionComponent)
	{
		m_CollisionComponent->SetCollisionEventForLayer(NameHash("Asteroid"), this,
			&PlayerShip::OnHitAsteroid);
		m_CollisionComponent->SetCollider(new CircleCollider(14.0f));
		//m_CollisionComponent->m_DebugDraw = true;
	}

	m_MoveComponent = AddComponent<MoveComponent>();

	m_SpriteComponent = EmplaceComponent<SpriteComponent>("Fighter1");
	if (m_SpriteComponent)
	{
		m_SpriteComponent->m_Sprite.setRotation(45.0f);
		m_SpriteComponent->m_Sprite.setOrigin( { 16.0f, 16.0f } );
	}

	m_OutsideComponent = AddComponent<OutsideComponent>();
	if (m_OutsideComponent)
	{
		m_OutsideComponent->SetResponse(OutsideComponent::Response::Wrap);
		m_OutsideComponent->SetReferenceSprite(m_SpriteComponent);
	}

	GameApp::GetInputEventManager().GetKeyPressedEvent(sf::Keyboard::Space).AddDelegate(this,
		&PlayerShip::OnSpacePressed);

	m_IsInvincible = true;
	GetWorld()->GetSystem<TimerSystem>()->StartTimer(m_InvincibilityTime,
		ObjectWeakRef(this), &PlayerShip::OnInvincibilityExpire);
}

void PlayerShip::Tick(float deltaTime)
{
	GameObject::Tick(deltaTime);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		m_SpriteComponent->m_Sprite.rotate(-m_RotationSpeed * deltaTime);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		m_SpriteComponent->m_Sprite.rotate(m_RotationSpeed * deltaTime);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		Thrust(m_Acceleration, deltaTime);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		Thrust(-m_Acceleration, deltaTime);
	}
}

float PlayerShip::GetRotation() const
{
	return m_SpriteComponent->m_Sprite.getRotation();
}

void PlayerShip::Thrust(float acceleration, float deltaTime)
{
	const Vec2 thrustUnit = Math::RotationToUnitVector(GetRotation());
	m_MoveComponent->AddVelocity(acceleration * deltaTime * thrustUnit);
}

void PlayerShip::OnHitAsteroid(GameObject* otherObject, CollisionComponent* otherComponent)
{
	if (!m_IsInvincible)
	{
		ExplodeObject(*this);
	}
}

void PlayerShip::OnSpacePressed(const sf::Event::KeyEvent& keyEvent)
{
	Laser* laser = GetWorld()->AddObject<Laser>();
	laser->SetPosition(GetPosition());
	laser->Shoot(GetRotation(), m_LaserSpeed);
}

void PlayerShip::OnInvincibilityExpire()
{
	m_IsInvincible = false;
}
