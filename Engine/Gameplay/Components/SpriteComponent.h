#pragma once

#include "Engine/Core.h"
#include "Engine/Gameplay/Component.h"

#include "SFML/Graphics/Sprite.hpp"

class SpriteComponent : public Component
{
public:
	SpriteComponent(NameHash textureName);
	MACRO_DeclareTypeInfo(SpriteComponent)

	void SetVisible(bool visibility) { m_Visible = visibility; };

	enum class AnimationMode
	{
		None,
		SubimagesPerTick,
		SubimagesPerSecond
	};

	void SetupSubimages(IntVec2 subimageSize, IntVec2 subimageSpacing, int numSubimages, int subimagesPerRow, int currentSubimage);
	void Animate(AnimationMode mode = AnimationMode::SubimagesPerTick, float rate = 1.0f, bool loop = true);
	MulticastEvent<> EventAnimationEnd;

	virtual void Tick(float deltaTime) override;
	virtual void Draw(sf::RenderTarget& renderTarget, const sf::RenderStates& renderStates) const override;

	// Public to allow changing local transform, etc.
	sf::Sprite m_Sprite;

private:
	void AdvanceSubimage(int framesToAdvance);
	void UpdateSpriteRect();

	bool m_Visible = true;

	bool m_SubimagesEnabled = false;
	IntVec2 m_SubimageSize = {32, 32};
	IntVec2 m_SubimageSpacing = {0, 0};
	int m_NumSubimages = 1;
	int m_SubimagesPerRow = 1;
	int m_CurrentSubimage = 0;

	AnimationMode m_AnimationMode = AnimationMode::None;
	bool m_AnimationLoop = true;
	float m_AnimationRate = 1.0f;
	float m_AnimationCounter = 0.0f;
};
