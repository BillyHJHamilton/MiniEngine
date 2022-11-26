#include "Asteroid.h"

class LargeAsteroid : public Asteroid
{
protected:
	virtual void OnDestroy() override;
};

class MediumAsteroid : public Asteroid
{
public:
	virtual void Init() override;
protected:
	virtual void OnDestroy() override;
};

class SmallAsteroid : public Asteroid
{
public:
	virtual void Init() override;
};
