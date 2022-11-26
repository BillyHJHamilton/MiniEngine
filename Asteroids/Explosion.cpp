#include "Explosion.h"

#include "Engine/Gameplay/Components/SpriteComponent.h"
#include "Engine/Gameplay/World.h"

void Explosion::Init()
{
	m_SpriteComponent = EmplaceComponent<SpriteComponent>("Explode64");
	if (m_SpriteComponent)
	{
		m_SpriteComponent->SetupSubimages({ 64,64 }, { 0,0 }, 9, 5, 0);
		m_SpriteComponent->m_Sprite.setOrigin(32, 32);
		m_SpriteComponent->Animate(SpriteComponent::AnimationMode::SubimagesPerSecond, 16.0f, true);
		m_SpriteComponent->EventAnimationEnd.AddDelegate(this, &Explosion::OnAnimationEnd);
	}
}

void Explosion::OnAnimationEnd()
{
	Destroy();
}

void SpawnExplosion(World& world, Vec2 position)
{
	Explosion* explosion = world.AddObject<Explosion>();
	explosion->SetPosition(position);
}

void ExplodeObject(GameObject& object)
{
	SpawnExplosion(*object.GetWorld(), object.GetPosition());
	object.Destroy();
}
