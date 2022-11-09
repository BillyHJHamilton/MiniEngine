#include "SpriteComponent.h"

#include "Engine/AssetManager.h"
#include "Engine/Gameplay/GameObject.h"
#include "SFML/Graphics/RenderTarget.hpp"

TypeInfo const SpriteComponent::s_TypeInfo = TypeInfo("SpriteComponent", {"Component"});

SpriteComponent::SpriteComponent(NameHash TextureName)
{
	const sf::Texture* SpriteTexture = g_AssetManager.FindTexture(TextureName);
	if (SpriteTexture != nullptr)
	{
		m_Sprite.setTexture(*SpriteTexture);
	}
}

void SpriteComponent::SetupSubimages(IntVec2 subimageSize, IntVec2 subimageSpacing,
	int numSubimages, int subimagesPerRow, int currentSubimage)
{
	m_SubimagesEnabled = true;
	m_SubimageSize = subimageSize;
	m_SubimageSpacing = subimageSpacing;
	m_NumSubimages = numSubimages;
	m_SubimagesPerRow = subimagesPerRow;
	m_CurrentSubimage = currentSubimage;

	UpdateSpriteRect();
}

void SpriteComponent::Animate(AnimationMode mode, float rate, bool loop)
{
	m_AnimationMode = mode;
	m_AnimationLoop = loop;
	m_AnimationRate = rate;
}

void SpriteComponent::Tick(float deltaTime)
{
	if (m_AnimationMode != AnimationMode::None)
	{
		if (m_AnimationMode == AnimationMode::SubimagesPerTick)
		{
			m_AnimationCounter += m_AnimationRate;
		}
		else if (m_AnimationMode == AnimationMode::SubimagesPerSecond)
		{
			m_AnimationCounter += m_AnimationRate * deltaTime;
		}
		
		int framesToAdvance = static_cast<int>(m_AnimationCounter);
		if (framesToAdvance > 0)
		{
			m_AnimationCounter -= framesToAdvance;
			AdvanceSubimage(framesToAdvance);
			UpdateSpriteRect();
		}
	}
}

void SpriteComponent::Draw(sf::RenderTarget& RenderTarget, const sf::RenderStates& renderStates) const
{
	if (m_Visible && m_Sprite.getTexture() != nullptr)
	{
		RenderTarget.draw(m_Sprite, renderStates);
	}
}

// Todo: Could support backwards animation.
void SpriteComponent::AdvanceSubimage(int framesToAdvance)
{
	m_CurrentSubimage += framesToAdvance;
	while (m_CurrentSubimage >= m_NumSubimages)
	{
		EventAnimationEnd.Broadcast();
		m_CurrentSubimage -= m_NumSubimages;
	}
}

void SpriteComponent::UpdateSpriteRect()
{
	int row = m_CurrentSubimage / m_SubimagesPerRow;
	int col = m_CurrentSubimage % m_SubimagesPerRow;

	int x = col * (m_SubimageSize.x + m_SubimageSpacing.x);
	int y = row * (m_SubimageSize.y + m_SubimageSpacing.y);

	m_Sprite.setTextureRect( {x, y, m_SubimageSize.x, m_SubimageSize.y} );
}
