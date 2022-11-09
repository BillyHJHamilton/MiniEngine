#include "MoveComponent.h"

#include "Engine/Gameplay/GameObject.h"

TypeInfo const MoveComponent::s_TypeInfo = TypeInfo("MoveComponent", {"Component"});

void MoveComponent::SetVelocity(Vec2 newVelocity)
{
	m_Velocity = newVelocity;
	EventVelocityChanged.Broadcast(newVelocity);
}

void MoveComponent::AddVelocity(Vec2 relativeVelocity)
{
	SetVelocity(m_Velocity + relativeVelocity);
}

void MoveComponent::ClearVelocity()
{
	SetVelocity( {0.0f, 0.0f} );
}

void MoveComponent::SetAcceleration(Vec2 newAcceleration)
{
	m_Acceleration = newAcceleration;
}

void MoveComponent::AddAcceleration(Vec2 relativeAcceleration)
{
	SetAcceleration(m_Acceleration + relativeAcceleration);
}

void MoveComponent::ClearAcceleration()
{
	SetAcceleration( {0.0f, 0.0f} );
}

void MoveComponent::Tick(float deltaTime)
{
	assert(GetOwner() != nullptr);
	AddVelocity(m_Acceleration);
	GetOwner()->MovePosition(deltaTime * m_Velocity);
}
