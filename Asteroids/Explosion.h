#pragma once

#include "Engine/Gameplay/GameObject.h"

class SpriteComponent;
class World;

class Explosion : public GameObject
{
public:
	virtual void Init() override;

protected:
	void OnAnimationEnd();

	SpriteComponent* m_SpriteComponent = nullptr;
};

void ExplodeObject(GameObject& object);
void SpawnExplosion(World& world, Vec2 position);
