#pragma once

#include "Engine/Gameplay/GameObject.h"

class CollisionComponent;
class MoveComponent;
class OutsideComponent;
class SpriteComponent;

class Laser : public GameObject
{
public:
	virtual void Init() override;

	void Shoot(float sfRotation, float speed);

protected:
	void OnHitAsteroid(GameObject* otherObject, CollisionComponent* otherComponent);

	CollisionComponent* m_CollisionComponent = nullptr;
	MoveComponent* m_MoveComponent = nullptr;
	OutsideComponent* m_OutsideComponent = nullptr;
	SpriteComponent* m_SpriteComponent = nullptr;
};
