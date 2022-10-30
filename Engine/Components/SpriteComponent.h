#pragma once

#include "Engine\Core.h"
#include "Engine\Component.h"

#include "SFML/Graphics/Sprite.hpp"

class SpriteComponent : public Component
{
public:
	SpriteComponent(NameHash TextureName);
	static NameHash StaticType() { return NameHash("SpriteComponent"); }
	virtual NameHash Type() { return SpriteComponent::StaticType(); }

	virtual void OnAddedToObject(GameObject* newOwner) override;

	void OnOwnerPositionChanged(Vec2 newPosition);

	virtual void Draw(sf::RenderTarget& RenderTarget) const override;
private:
	sf::Sprite m_Sprite;
};
