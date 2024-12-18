#pragma once

#include "Engine/Core.h"
#include "SFML/Graphics/RenderStates.hpp"

class Component : public ITypeInfoProvider
{
public:
	Component();
	virtual ~Component();
	MACRO_DeclareTypeInfo(Component)

	virtual void OnAddedToObject(GameObject* newOwner) {};

	const GameObject* GetOwner() const { return m_Owner; }
	GameObject* GetOwner() { return m_Owner; }

	const World* GetWorld() const;
	World* GetWorld();

	virtual void Tick(float deltaTime) {};
	virtual void Draw(sf::RenderTarget& renderTarget, const sf::RenderStates& renderStates) const {};

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
