#pragma once

#include "Engine/Core.h"

enum class ECollisionResult : int8_t
{
	Unresolved = -1,
	NoCollision = 0,
	Collision = 1
};

class ICollider
{
public:
	virtual NameHash Type() const = 0;
	virtual ECollisionResult ResolveCollision(const ICollider* other) const = 0;

	// Public property for all colliders.
	Vec2 m_Position = {0.0f, 0.0f};

	static constexpr float Epsilon = 0.00001f;
};

