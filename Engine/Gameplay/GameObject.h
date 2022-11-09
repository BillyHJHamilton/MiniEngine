#pragma once

#include "Engine/Core.h"
#include "Engine/Gameplay/Component.h"

class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

	WeakRef<GameObject> GetWeakRef() { return m_RefTracker.MakeReference(this); }
	WeakRef<const GameObject> GetWeakRef() const { return m_RefTracker.MakeReference(const_cast<const GameObject*>(this)); }

	World* GetWorld() { return m_World; }
	const World* GetWorld() const { return m_World; }

	Component* AddComponent(Component* newComponent);
	Component* FindComponentByType(NameHash typeName);
	const Component* FindComponentByType(NameHash typeName) const;

	template<typename ComponentType> ComponentType* FindComponent();
	template<typename ComponentType> const ComponentType* FindComponent() const;

	WeakRef<Component> FindComponentWeakRefByType(NameHash typeName);
	WeakRef<const Component> FindComponentWeakRefByType(NameHash typeName) const;

	template<typename ComponentType> WeakRef<ComponentType> FindComponentWeakRef();
	template<typename ComponentType> WeakRef<const ComponentType> FindComponentWeakRef() const;

template<typename ComponentType> WeakRef<ComponentType> MakeComponentWeakRef(ComponentType* component);
	template<typename ComponentType> WeakRef<const ComponentType> MakeComponentWeakRef(const ComponentType* component) const;

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

	bool m_DrawOrigin = false;

private:
	friend class World;
	void SetWorld(World* NewWorld) { m_World = NewWorld; }

	RefTracker m_RefTracker;
	World* m_World = nullptr;
	std::vector<std::unique_ptr<Component>> m_ComponentList;
	Vec2 m_Position = {0,0};
	std::vector<NameHash> m_TagList;

#if DEBUG_MEMORY
	static int s_NumCreated;
	static int s_NumDestroyed;
#endif
};

// Template implementations

template<typename ComponentType>
ComponentType* GameObject::FindComponent()
{
	return static_cast<ComponentType*>(FindComponentByType(ComponentType::StaticType()));
}

template<typename ComponentType>
const ComponentType* GameObject::FindComponent() const
{
	return static_cast<const ComponentType*>(FindComponentByType(ComponentType::StaticType()));
}

template<typename ComponentType>
WeakRef<ComponentType> GameObject::FindComponentWeakRef()
{
	ComponentType* component = FindComponent<ComponentType>();
	return (component) ? m_RefTracker.MakeReference(component) : WeakRef<ComponentType>();
}

template<typename ComponentType>
WeakRef<const ComponentType> GameObject::FindComponentWeakRef() const
{
	const ComponentType* component = FindComponent<ComponentType>();
	return (component) ? m_RefTracker.MakeReference(component) : WeakRef<const ComponentType>();
}

template<typename ComponentType>
WeakRef<ComponentType> GameObject::MakeComponentWeakRef(ComponentType* component)
{
	assert(component->GetOwner() == this);
	return m_RefTracker.MakeReference(component);
}

template<typename ComponentType>
WeakRef<const ComponentType> GameObject::MakeComponentWeakRef(const ComponentType* component) const
{
	assert(component->GetOwner() == this);
	return m_RefTracker.MakeReference(component);
}
