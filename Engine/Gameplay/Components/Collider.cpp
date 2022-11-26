#include "Collider.h"

#include "Engine/Math.h"
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderTarget.hpp"

TypeInfo const PointCollider::s_TypeInfo = TypeInfo("PointCollider", {});
TypeInfo const CircleCollider::s_TypeInfo = TypeInfo("CircleCollider", {});
TypeInfo const AlignedBoxCollider::s_TypeInfo = TypeInfo("AlignedBoxCollider", {});

ECollisionResult PointCollider::ResolveCollision(const ICollider* other) const
{
	assert(other);

	switch(other->GetTypeName())
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
	// Points are infinitely small and cannot collide with one another.
	return ECollisionResult::NoCollision;
}

ECollisionResult CircleCollider::ResolveCollision(const ICollider* other) const
{
	assert(other);

	switch(other->GetTypeName())
	{
		case NameHash::StaticHash("PointCollider"):
			return WithPoint(static_cast<const PointCollider&>(*other));
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
	return (Math::DistanceSquared(m_Position, other.m_Position) <= squaredCollisionDistance)
		? ECollisionResult::Collision
		: ECollisionResult::NoCollision;
}

ECollisionResult CircleCollider::WithCircle(const CircleCollider& other) const
{
	float const collisionDistance = m_Radius + other.m_Radius;
	float const squaredCollisionDistance = collisionDistance*collisionDistance + ICollider::Epsilon;
	return (Math::DistanceSquared(m_Position, other.m_Position) <= squaredCollisionDistance)
		? ECollisionResult::Collision
		: ECollisionResult::NoCollision;
}

ECollisionResult AlignedBoxCollider::ResolveCollision(const ICollider* other) const
{
	assert(other);

	switch(other->GetTypeName())
	{
		case NameHash::StaticHash("PointCollider"):
			return WithPoint(static_cast<const PointCollider&>(*other));
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
	return Math::DistanceSquared(nearestPoint, other.m_Position) <= squaredCollisionDistance
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

// Debug drawing

void PointCollider::DebugDraw(sf::RenderTarget& renderTarget, const sf::RenderStates& renderStates) const
{
	sf::RectangleShape debugRectangle;
	debugRectangle.setFillColor(sf::Color::Green);
	debugRectangle.setOutlineColor(sf::Color::Transparent);
	debugRectangle.setPosition(m_Position - Vec2{1.0f, 1.0f});
	debugRectangle.setSize({2.0f,2.0f});
	renderTarget.draw(debugRectangle);
}

void CircleCollider::DebugDraw(sf::RenderTarget& renderTarget, const sf::RenderStates& renderStates) const
{
	sf::CircleShape debugCircle;
	debugCircle.setFillColor(sf::Color::Transparent);
	debugCircle.setOutlineColor(sf::Color::Green);
	debugCircle.setOutlineThickness(1.0f);
	debugCircle.setPosition(m_Position - Vec2{m_Radius, m_Radius});
	debugCircle.setRadius(m_Radius);
	renderTarget.draw(debugCircle);
}

void AlignedBoxCollider::DebugDraw(sf::RenderTarget& renderTarget, const sf::RenderStates& renderStates) const
{
	sf::RectangleShape debugRectangle;
	debugRectangle.setFillColor(sf::Color::Transparent);
	debugRectangle.setOutlineColor(sf::Color::Green);
	debugRectangle.setOutlineThickness(1.0f);
	debugRectangle.setPosition(m_Position);
	debugRectangle.setSize(m_Extent);
	renderTarget.draw(debugRectangle);
}
