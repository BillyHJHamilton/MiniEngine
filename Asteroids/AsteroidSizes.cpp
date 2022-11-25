#include "AsteroidSizes.h"
#include "Engine/Gameplay.h"

void LargeAsteroid::OnDestroy()
{
	SpawnObjectAtObject<MediumAsteroid>(*this);
	SpawnObjectAtObject<MediumAsteroid>(*this);
}

void MediumAsteroid::Init()
{
	m_Scale = 0.75f;
	Asteroid::Init();
}

void MediumAsteroid::OnDestroy()
{
	SpawnObjectAtObject<SmallAsteroid>(*this);
	SpawnObjectAtObject<SmallAsteroid>(*this);
}

void SmallAsteroid::Init()
{
	m_Scale = 0.5f;
	Asteroid::Init();
}
