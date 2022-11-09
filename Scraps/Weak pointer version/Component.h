#pragma once

#include "Core.h"
#include "SFML/Graphics/RenderStates.hpp"

class Component : public std::enable_shared_from_this<Component>
{
public:
	virtual ~Component() {};
	static NameHash StaticType() { return NameHash("Component"); }
	virtual NameHash Type() { return Component::StaticType(); }

	std::weak_ptr<const Component> GetWeakPtr() const { return weak_from_this(); }
	std::weak_ptr<Component> GetWeakPtr() { return weak_from_this(); }

	template<typename DerivedComponentType>
	std::weak_ptr<const DerivedComponentType> GetWeakPtr() const
	{
		return std::static_pointer_cast<const DerivedComponentType>(GetWeakPtr().lock());
	}

	template<typename DerivedComponentType>
	std::weak_ptr<DerivedComponentType> GetWeakPtr()
	{
		return std::static_pointer_cast<DerivedComponentType>(GetWeakPtr().lock());
	}

	virtual void OnAddedToObject(GameObject* newOwner) {};

	const GameObject* GetOwner() const { return m_Owner; }
	GameObject* GetOwner() { return m_Owner; }

	const World* GetWorld() const;
	World* GetWorld();

	virtual void Tick(float DeltaTime) {};
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
