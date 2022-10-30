#pragma once

#include "Engine\Core.h"
#include "Engine\Component.h"

class MoveComponent : public Component
{
public:
	static NameHash StaticType() { return NameHash("MoveComponent"); }
	virtual NameHash Type() { return MoveComponent::StaticType(); }

	void SetVelocity(Vec2 newVelocity);
	void AddVelocity(Vec2 relativeVelocity);
	void ClearVelocity();
	MulticastEvent<Vec2> EventVelocityChanged;

	void SetAcceleration(Vec2 newAcceleration);
	void AddAcceleration(Vec2 relativeAcceleration);
	void ClearAcceleration();

	virtual void Tick(float DeltaTime) override;
private:
	Vec2 m_Velocity;
	Vec2 m_Acceleration;
};
