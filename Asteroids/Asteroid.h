#pragma once

#include "Engine/Gameplay/GameObject.h"

class CollisionComponent;
class MoveComponent;
class OutsideComponent;
class SpriteComponent;

class Asteroid : public GameObject
{
public:
	virtual void Init() override;
	virtual void OnDestroy() override;

protected:
	CollisionComponent* m_CollisionComponent = nullptr;
	MoveComponent* m_MoveComponent = nullptr;
	SpriteComponent* m_SpriteComponent = nullptr;
	OutsideComponent* m_OutsideComponent = nullptr;

	float m_FloatSpeed = 80.0f; // Pixels per second
	float m_Scale = 1.0f;
};

