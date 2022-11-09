#pragma once

#include "Engine/Gameplay.h"

// To do:
// - Get size of world automatically
// - Deal properly with the size of the object (sprite size, for example)
// - Maybe only do this on a special "outside world" event

class WrapAroundComponent : public Component
{
public:
	//static NameHash StaticType() { return NameHash("WrapAroundComponent"); }
	//virtual NameHash Type() { return WrapAroundComponent::StaticType(); }
	MACRO_DeclareTypeInfo(WrapAroundComponent)

	virtual void Tick(float DeltaTime) override
	{
		assert(GetOwner());

		while (m_BoundSize.x > 0.0f && GetOwner()->GetPosition().x > m_BoundSize.x)
		{
			GetOwner()->MovePosition({-m_BoundSize.x, 0.0f});
		}

		while (m_BoundSize.y > 0.0f && GetOwner()->GetPosition().y > m_BoundSize.y)
		{
			GetOwner()->MovePosition({0.0f, -m_BoundSize.y});
		}

		while (m_BoundSize.x > 0.0f && GetOwner()->GetPosition().x < 0.0f)
		{
			GetOwner()->MovePosition({m_BoundSize.x, 0.0f});
		}

		while (m_BoundSize.y > 0.0f && GetOwner()->GetPosition().y < 0.0f)
		{
			GetOwner()->MovePosition({0.0f, m_BoundSize.y});
		}
	}

	Vec2 m_BoundSize;
};
