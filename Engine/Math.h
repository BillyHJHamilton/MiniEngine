#pragma once

#include "Engine/Core.h"
#include <cmath>

namespace Math
{
	inline float LengthSquared(Vec2 v)
	{
		return v.x*v.x + v.y*v.y;
	}
	inline float Length(Vec2 v)
	{
		return std::sqrt(LengthSquared(v));
	}
	inline float DistanceSquared(Vec2 p1, Vec2 p2)
	{
		return LengthSquared(p2 - p1);
	}
	inline float Distance(Vec2 p1, Vec2 p2)
	{
		return Length(p2 - p1);
	}
	inline float DegToRad(float degrees)
	{
		return 0.01745329252f * degrees;
	}
	inline float SinDeg(float degrees)
	{
		return std::sinf(DegToRad(degrees));
	}
	inline float CosDeg(float degrees)
	{
		return std::cosf(DegToRad(degrees));
	}
	inline Vec2 RotationToUnitVector(float sfRotation)
	{
		return
		{
			Math::SinDeg(sfRotation),
			-Math::CosDeg(sfRotation)
		};
	}
};
