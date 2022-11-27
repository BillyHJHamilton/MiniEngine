#pragma once

#include "Engine/Gameplay/GameObject.h"
#include "SFML/Window/Event.hpp"

class CollisionComponent;
class MoveComponent;
class SpriteComponent;
class OutsideComponent;

class PlayerShip : public GameObject
{
public:
	virtual void Init() override;

	virtual void Tick(float deltaTime) override;

protected:
	float GetRotation() const;

	void Thrust(float acceleration, float deltaTime);

	CollisionComponent* m_CollisionComponent = nullptr;
	MoveComponent* m_MoveComponent = nullptr;
	SpriteComponent* m_SpriteComponent = nullptr;
	OutsideComponent* m_OutsideComponent = nullptr;

	const float m_RotationSpeed = 270.0f; // Degrees per second
	const float m_Acceleration = 250.0f; // Pixels per second squared
	const float m_LaserSpeed = 500.0f;

private:
	void OnHitAsteroid(GameObject* otherObject, CollisionComponent* otherComponent);
	void OnSpacePressed(const sf::Event::KeyEvent& keyEvent);
};
