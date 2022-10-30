#pragma once

#include "Engine/Core.h"
#include "Collider_i.h"

class PointCollider : public ICollider
{
public:
	static NameHash StaticType() { return NameHash("PointCollider"); }
	virtual NameHash Type() const { return StaticType(); }

	PointCollider() = default;

	virtual ECollisionResult ResolveCollision(const ICollider* other) const;
	ECollisionResult WithPoint(const PointCollider& other) const;
};

class CircleCollider : public ICollider
{
public:
	static NameHash StaticType() { return NameHash("CircleCollider"); }
	virtual NameHash Type() const { return StaticType(); }

	CircleCollider() = default;
	CircleCollider(float radius) : m_Radius(radius) {}

	virtual ECollisionResult ResolveCollision(const ICollider* other) const;
	ECollisionResult WithPoint(const PointCollider& other) const;
	ECollisionResult WithCircle(const CircleCollider& other) const;

	float m_Radius = 1.0f;
};

class AlignedBoxCollider : public ICollider
{
public:
	static NameHash StaticType() { return NameHash("AlignedBoxCollider"); }
	virtual NameHash Type() const { return StaticType(); }

	AlignedBoxCollider(Vec2 extent) : m_Extent(extent) {}

	virtual ECollisionResult ResolveCollision(const ICollider* other) const;
	ECollisionResult WithPoint(const PointCollider& other) const;
	ECollisionResult WithCircle(const CircleCollider& other) const;
	ECollisionResult WithAlignedBox(const AlignedBoxCollider& other) const;

	Vec2 m_Extent = {1.0f, 1.0f};

	Vec2 Min() const { return m_Position; }
	Vec2 Max() const { return m_Position + m_Extent; }
};
