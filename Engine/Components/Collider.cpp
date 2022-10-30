#include "Engine/Components/Collider.h"
#include "Engine/Math.h"

#include <algorithm>

ECollisionResult PointCollider::ResolveCollision(const ICollider* other) const
{
	assert(other);

	switch(other->Type())
	{
		case NameHash::StaticHash("PointCollider"):
			return WithPoint(static_cast<const PointCollider&>(*other));
		case NameHash::StaticHash("CircleCollider"):
			return static_cast<const CircleCollider*>(other)->WithPoint(*this);
		case NameHash::StaticHash("AlignedBoxCollider"):
			return static_cast<const AlignedBoxCollider*>(other)->WithPoint(*this);
		default:
			return ECollisionResult::Unresolved;
	}
}

ECollisionResult PointCollider::WithPoint(const PointCollider& other) const
{
	return ECollisionResult::NoCollision;

	// Points never collide with other points.
	/*	return (m_Position.x > other.m_Position.x - ICollider::Epsilon &&
		    m_Position.x < other.m_Position.x + ICollider::Epsilon &&
		    m_Position.y > other.m_Position.y - ICollider::Epsilon &&
		    m_Position.y < other.m_Position.y + ICollider::Epsilon)
		? ECollisionResult::Collision
		: ECollisionResult::NoCollision;*/
}

ECollisionResult CircleCollider::ResolveCollision(const ICollider* other) const
{
	assert(other);

	switch(other->Type())
	{
		case NameHash::StaticHash("CircleCollider"):
			return WithCircle(static_cast<const CircleCollider&>(*other));
		case NameHash::StaticHash("AlignedBoxCollider"):
			return static_cast<const AlignedBoxCollider*>(other)->WithCircle(*this);
		default:
			return ECollisionResult::Unresolved;
	}
}

ECollisionResult CircleCollider::WithPoint(const PointCollider& other) const
{
	float const squaredCollisionDistance = m_Radius*m_Radius + ICollider::Epsilon;
	return (DistanceSquared(m_Position, other.m_Position) <= squaredCollisionDistance)
		? ECollisionResult::Collision
		: ECollisionResult::NoCollision;
}

ECollisionResult CircleCollider::WithCircle(const CircleCollider& other) const
{
	float const collisionDistance = m_Radius + other.m_Radius;
	float const squaredCollisionDistance = collisionDistance*collisionDistance + ICollider::Epsilon;
	return (DistanceSquared(m_Position, other.m_Position) <= squaredCollisionDistance)
		? ECollisionResult::Collision
		: ECollisionResult::NoCollision;
}

ECollisionResult AlignedBoxCollider::ResolveCollision(const ICollider* other) const
{
	assert(other);

	switch(other->Type())
	{
		case NameHash::StaticHash("CircleCollider"):
			return WithCircle(static_cast<const CircleCollider&>(*other));
		case NameHash::StaticHash("AlignedBoxCollider"):
			return WithAlignedBox(static_cast<const AlignedBoxCollider&>(*other));
		default:
			return ECollisionResult::Unresolved;
	}
}

ECollisionResult AlignedBoxCollider::WithPoint(const PointCollider& other) const
{
	if (other.m_Position.x + ICollider::Epsilon >= Max().x ||
		other.m_Position.y + ICollider::Epsilon >= Max().y ||
		other.m_Position.x - ICollider::Epsilon <= Min().x ||
		other.m_Position.y - ICollider::Epsilon <= Min().y)
	{
		return ECollisionResult::NoCollision;
	}
	else
	{
		return ECollisionResult::Collision;
	}
}

ECollisionResult AlignedBoxCollider::WithCircle(const CircleCollider& other) const
{
	Vec2 const nearestPoint = {
		std::clamp(other.m_Position.x, Min().x, Max().x),
		std::clamp(other.m_Position.y, Min().y, Max().y)
	};
	float const collisionDistance = other.m_Radius + other.m_Radius;
	float const squaredCollisionDistance = collisionDistance*collisionDistance + ICollider::Epsilon;
	return DistanceSquared(nearestPoint, other.m_Position) <= squaredCollisionDistance
		? ECollisionResult::Collision
		: ECollisionResult::NoCollision;
}

ECollisionResult AlignedBoxCollider::WithAlignedBox(const AlignedBoxCollider& other) const
{
	if (other.Min().x + ICollider::Epsilon >= Max().x ||
		other.Min().y + ICollider::Epsilon >= Max().y ||
		other.Max().x - ICollider::Epsilon <= Min().x ||
		other.Max().y - ICollider::Epsilon <= Min().y)
	{
		return ECollisionResult::NoCollision;
	}
	else
	{
		return ECollisionResult::Collision;
	}
}
