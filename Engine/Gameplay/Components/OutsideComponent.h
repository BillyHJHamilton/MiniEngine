#pragma once

#include "Engine/Gameplay/Component.h"

class SpriteComponent;

class OutsideComponent : public Component
{
public:
	MACRO_DeclareTypeInfo(OutsideComponent)

	// Set a sprite to use for detecting if the object is outside.
	void SetReferenceSprite(const SpriteComponent* spriteComponent);

	enum class Response : unsigned char
	{
		None,
		Wrap,
		Destroy
	};

	// Define what will happen to the object if it goes outside the world boundary.
	void SetResponse(Response response);

	virtual void Tick(float deltaTime) override;

	MulticastEvent<GameObject*> EventOutside;

private:
	void TriggerOutside(const sf::FloatRect& worldBox, const sf::FloatRect& objectBox);
	void OutsideWrap(const sf::FloatRect& worldBox, const sf::FloatRect& objectBox);

	WeakRef<const SpriteComponent> m_ReferenceSprite;
	Response m_Response = Response::None;
};
