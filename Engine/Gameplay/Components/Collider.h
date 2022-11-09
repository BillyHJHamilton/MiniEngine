#pragma once

#include "Engine/Core.h"
#include "Collider_i.h"

class PointCollider : public ICollider
{
public:
	MACRO_DeclareTypeInfo(PointCollider)
	PointCollider() = default;

	virtual ECollisionResult ResolveCollision(const ICollider* other) const;
	ECollisionResult WithPoint(const PointCollider& other) const;

	virtual void DebugDraw(sf::RenderTarget& renderTarget, const sf::RenderStates& renderStates) const;
};

class CircleCollider : public ICollider
{
public:
	MACRO_DeclareTypeInfo(CircleCollider)
	CircleCollider() = default;
	CircleCollider(float radius) : m_Radius(radius) {}

	virtual ECollisionResult ResolveCollision(const ICollider* other) const;
	ECollisionResult WithPoint(const PointCollider& other) const;
	ECollisionResult WithCircle(const CircleCollider& other) const;

	virtual void DebugDraw(sf::RenderTarget& renderTarget, const sf::RenderStates& renderStates) const;

	float m_Radius = 1.0f;
};

class AlignedBoxCollider : public ICollider
{
public:
	MACRO_DeclareTypeInfo(AlignedBoxCollider)
	AlignedBoxCollider(Vec2 extent) : m_Extent(extent) {}

	virtual ECollisionResult ResolveCollision(const ICollider* other) const;
	ECollisionResult WithPoint(const PointCollider& other) const;
	ECollisionResult WithCircle(const CircleCollider& other) const;
	ECollisionResult WithAlignedBox(const AlignedBoxCollider& other) const;

	virtual void DebugDraw(sf::RenderTarget& renderTarget, const sf::RenderStates& renderStates) const;

	Vec2 m_Extent = {1.0f, 1.0f};

	Vec2 Min() const { return m_Position; }
	Vec2 Max() const { return m_Position + m_Extent; }
};
