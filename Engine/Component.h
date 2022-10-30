#pragma once

#include "Core.h"

class Component
{
public:
	virtual ~Component() {};
	static NameHash StaticType() { return NameHash("Component"); }
	virtual NameHash Type() { return Component::StaticType(); }

	virtual void OnAddedToObject(GameObject* newOwner) {};

	const GameObject* GetOwner() const { return m_Owner; }
	GameObject* GetOwner() { return m_Owner; }

	const World* GetWorld() const;
	World* GetWorld();

	virtual void Tick(float DeltaTime) {};
	virtual void Draw(sf::RenderTarget& renderTarget) const {};

#if DEBUG_MEMORY
	static void CheckMemoryReleased();
#endif

private:
	friend class GameObject;
	void SetOwner(GameObject* newOwner);

	GameObject* m_Owner = nullptr;

#if DEBUG_MEMORY
	static int s_NumCreated;
	static int s_NumDestroyed;
#endif
};
