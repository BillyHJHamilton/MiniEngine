#include "PlayerShip.h"

#include "Asteroids/Explosion.h"
#include "Asteroids/Laser.h"
#include "Engine/App/GameApp.h"
#include "Engine/Gameplay/Components/CollisionComponent.h"
#include "Engine/Gameplay/Components/MoveComponent.h"
#include "Engine/Gameplay/Components/SpriteComponent.h"
#include "Engine/Gameplay/Components/WrapAroundComponent.h"
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
	if (m_MoveComponent)
	{
		m_MoveComponent->SetVelocity( {0.0f, 25.0f} );
	}

	m_SpriteComponent = EmplaceComponent<SpriteComponent>("Fighter1");
	if (m_SpriteComponent)
	{
		m_SpriteComponent->m_Sprite.setRotation(45.0f);
		m_SpriteComponent->m_Sprite.setOrigin( { 16.0f, 16.0f } );
	}

	m_WrapAroundComponent = AddComponent<WrapAroundComponent>();
	if (m_WrapAroundComponent)
	{
		m_WrapAroundComponent->m_BoundSize = {640.0f, 480.0f};
	}

	GameApp::GetInputEventManager().GetKeyPressedEvent(sf::Keyboard::Space).AddDelegate(this,
		&PlayerShip::OnSpacePressed);
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
	assert(GetWorld());
	ExplodeObject(*this);
}

void PlayerShip::OnSpacePressed(const sf::Event::KeyEvent& keyEvent)
{
	Laser* laser = GetWorld()->AddObject<Laser>();
	laser->SetPosition(GetPosition());
	laser->Shoot(GetRotation(), m_LaserSpeed);
}
