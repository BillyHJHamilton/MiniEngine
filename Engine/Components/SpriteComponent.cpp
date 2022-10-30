#include "SpriteComponent.h"

#include "Engine/AssetManager.h"
#include "Engine/GameObject.h"
#include "SFML/Graphics/RenderTarget.hpp"

SpriteComponent::SpriteComponent(NameHash TextureName)
{
	const sf::Texture* SpriteTexture = g_AssetManager.FindTexture(TextureName);
	if (SpriteTexture != nullptr)
	{
		m_Sprite.setTexture(*SpriteTexture);
	}
}

void SpriteComponent::OnAddedToObject(GameObject* newOwner)
{
	newOwner->EventPositionChanged.AddDelegate(this, &SpriteComponent::OnOwnerPositionChanged);
}

void SpriteComponent::OnOwnerPositionChanged(Vec2 newPosition)
{
	m_Sprite.setPosition(newPosition);
}

void SpriteComponent::Draw(sf::RenderTarget& RenderTarget) const
{
	if (m_Sprite.getTexture() != nullptr)
	{
		RenderTarget.draw(m_Sprite);
	}
}
