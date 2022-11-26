#pragma once

#include "Core.h"
#include "Component.h"

class GameObject : public std::enable_shared_from_this<GameObject>
{
public:
	GameObject();
	virtual ~GameObject();

	std::weak_ptr<const GameObject> GetWeakPtr() const { return weak_from_this(); }
	std::weak_ptr<GameObject> GetWeakPtr() { return weak_from_this(); }

	World* GetWorld() { return m_World; }
	const World* GetWorld() const { return m_World; }

	Component* AddComponent(Component* newComponent);
	Component* FindComponentByType(NameHash typeName);
	const Component* FindComponentByType(NameHash typeName) const;

	template<typename ComponentType>
	ComponentType* FindComponent()
	{
		return static_cast<ComponentType*>(FindComponentByType(ComponentType::StaticType()));
	}

	template<typename ComponentType>
	const ComponentType* FindComponent() const
	{
		return static_cast<const ComponentType*>(FindComponentByType(ComponentType::StaticType()));
	}

	Vec2 GetPosition() const { return m_Position; }
	void SetPosition(Vec2 newPosition);
	void MovePosition(Vec2 relativeVector);
	MulticastEvent<Vec2> EventPositionChanged;

	void AddTag(NameHash newTag);
	void RemoveTag(NameHash tagToRemove);
	bool HasTag(NameHash tagToCheck) const;
	const std::vector<NameHash>& GetTags() const { return m_TagList; }
	MulticastEvent<NameHash> EventTagAdded;
	MulticastEvent<NameHash> EventTagRemoved;

	void Destroy();
	MulticastEvent<GameObject*> EventDestroy;

	virtual void Tick(float deltaTime);
	virtual void Draw(sf::RenderTarget& renderTarget) const;

#if DEBUG_MEMORY
	static void CheckMemoryReleased();
#endif

#if UNIT_TESTS
	static void UnitTest();
#endif

private:
	friend class World;
	void SetWorld(World* NewWorld) { m_World = NewWorld; }

	World* m_World = nullptr;
	std::vector<std::shared_ptr<Component>> m_ComponentList;
	Vec2 m_Position = {0,0};
	std::vector<NameHash> m_TagList;

#if DEBUG_MEMORY
	static int s_NumCreated;
	static int s_NumDestroyed;
#endif
};
