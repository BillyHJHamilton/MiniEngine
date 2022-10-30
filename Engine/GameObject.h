#pragma once

#include "Core.h"
#include "Component.h"

class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

	World* GetWorld() { return m_World; }
	const World* GetWorld() const { return m_World; }

	Component* AddComponent(Component* NewComponent);
	Component* FindComponentByType(NameHash TypeName);
	const Component* FindComponentByType(NameHash TypeName) const;

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

	virtual void Tick(float DeltaTime);
	virtual void Draw(sf::RenderTarget& RenderTarget) const;

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
	std::vector<std::unique_ptr<Component>> m_ComponentList;
	Vec2 m_Position = {0,0};
	std::vector<NameHash> m_TagList;

#if DEBUG_MEMORY
	static int s_NumCreated;
	static int s_NumDestroyed;
#endif
};
