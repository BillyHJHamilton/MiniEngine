#include "GameObject.h"

#include "Engine/Gameplay/Component.h"
#include "Engine/Gameplay/World.h"
#include "SFML/Graphics/RenderStates.hpp"

#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/RectangleShape.hpp"

GameObject::GameObject()
{
	#if DEBUG_MEMORY
		++s_NumCreated;
	#endif
}

GameObject::~GameObject()
{
	EventDestroy.Broadcast(this);

	#if DEBUG_MEMORY
		++s_NumDestroyed;
	#endif
}

#if DEBUG_MEMORY
int GameObject::s_NumCreated = 0;
int GameObject::s_NumDestroyed = 0;

void GameObject::CheckMemoryReleased()
{
	if (s_NumCreated == s_NumDestroyed)
	{
		std::cout << "Object Memory OK." << std::endl;
	}
	else
	{
		std::cerr << "Object Memory Error!  Created " << s_NumCreated
			<< ", Destroyed " << s_NumDestroyed << std::endl;
	}
}
#endif

Component* GameObject::AddComponent(Component* newComponent)
{
	assert(newComponent != nullptr);
	m_ComponentList.emplace_back(newComponent);
	newComponent->SetOwner(this);
	return newComponent;
}

const Component* GameObject::FindComponentByType(NameHash typeName) const
{
	for (const std::unique_ptr<Component>& NextComponent : m_ComponentList)
	{
		if (NextComponent->GetTypeName() == typeName)
		{
			return NextComponent.get();
		}
	}
	return nullptr;
}

Component* GameObject::FindComponentByType(NameHash typeName)
{
	return const_cast<Component*>(const_cast<const GameObject*>(this)->FindComponentByType(typeName));
}

WeakRef<Component> GameObject::FindComponentWeakRefByType(NameHash typeName)
{
	Component* component = FindComponentByType(typeName);
	return (component) ? m_RefTracker.MakeReference(component) : WeakRef<Component>();
}

WeakRef<const Component> GameObject::FindComponentWeakRefByType(NameHash typeName) const
{
	const Component* component = FindComponentByType(typeName);
	return (component) ? m_RefTracker.MakeReference(component) : WeakRef<const Component>();
}

void GameObject::SetPosition(Vec2 newPosition)
{
	m_Position = newPosition;
	EventPositionChanged.Broadcast(newPosition);
}

void GameObject::MovePosition(Vec2 relativeVector)
{
	SetPosition(m_Position + relativeVector);
}

void GameObject::AddTag(NameHash newTag)
{
	if (std::find(m_TagList.cbegin(), m_TagList.cend(), newTag) == m_TagList.end())
	{
		m_TagList.push_back(newTag);
		EventTagAdded.Broadcast(newTag);
	}
}

void GameObject::RemoveTag(NameHash tagToRemove)
{
	auto itr = std::find(m_TagList.begin(), m_TagList.end(), tagToRemove);
	if (itr != m_TagList.end())
	{
		*itr = m_TagList.back();
		m_TagList.pop_back();
		EventTagRemoved.Broadcast(tagToRemove);
	}
}

bool GameObject::HasTag(NameHash tagToCheck) const
{
	return std::find(m_TagList.cbegin(), m_TagList.cend(), tagToCheck)
		!= m_TagList.cend();
}

void GameObject::Destroy()
{
	assert(m_World != nullptr);
	m_World->DestroyObject(this);
}

void GameObject::Tick(float deltaTime)
{
	assert(m_World != nullptr);
	for (std::unique_ptr<Component>& NextComponent : m_ComponentList)
	{
		NextComponent->Tick(deltaTime);
	}
}

void GameObject::Draw(sf::RenderTarget& renderTarget) const
{
	assert(m_World != nullptr);

	sf::RenderStates renderStates;
	renderStates.transform.translate(m_Position);

	for (const std::unique_ptr<Component>& NextComponent : m_ComponentList)
	{
		NextComponent->Draw(renderTarget, renderStates);
	}

	if (m_DrawOrigin)
	{
		sf::RectangleShape debugRectangle;
		debugRectangle.setFillColor(sf::Color::Red);
		debugRectangle.setOutlineColor(sf::Color::Transparent);
		debugRectangle.setPosition(m_Position - Vec2{1.0f, 1.0f});
		debugRectangle.setSize({2.0f,2.0f});
		renderTarget.draw(debugRectangle);
	}
}

#if UNIT_TESTS
#include "World.h"
void GameObject::UnitTest()
{
	World tempWorld;
	GameObject* ship = tempWorld.AddObject(new GameObject());

	struct TempStruct
	{
		void SetVec(Vec2 inVec) { m_Vec = inVec; }
		Vec2 m_Vec;
	};
	TempStruct tempStruct;
	ship->EventPositionChanged.AddDelegate(&tempStruct, &TempStruct::SetVec);

	ship->SetPosition({200.0f,50.0f});
	assert(ship->GetPosition().x == 200.0f);
	assert(ship->GetPosition().y == 50.0f);
	assert(tempStruct.m_Vec.x == 200.0f);
	assert(tempStruct.m_Vec.y == 50.0f);

	ship->AddTag("Fast");
	ship->AddTag("Player");
	ship->RemoveTag("Fast");
	assert(!ship->HasTag("Fast"));
	assert(ship->HasTag("Player"));
}
#endif
