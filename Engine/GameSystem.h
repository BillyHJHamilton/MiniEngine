#pragma once

#include "Engine/Core.h"

class GameSystem
{
public:
	GameSystem();
	virtual ~GameSystem();
	static NameHash StaticType() { return NameHash("GameSystem"); }
	virtual NameHash Type() { return GameSystem::StaticType(); }

	World* GetWorld() { return m_World; }
	const World* GetWorld() const { return m_World; }

	virtual void Tick(float deltaTime) {};

#if DEBUG_MEMORY
	static void CheckMemoryReleased();
#endif

private:
	World* m_World;

#if DEBUG_MEMORY
	static int s_NumCreated;
	static int s_NumDestroyed;
#endif
};
