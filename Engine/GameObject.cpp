#include "GameObject.h"

#include "Component.h"
#include "World.h"

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

Component* GameObject::AddComponent(Component* NewComponent)
{
	assert(NewComponent != nullptr);
	m_ComponentList.emplace_back(NewComponent);
	NewComponent->SetOwner(this);
	return NewComponent;
}

const Component* GameObject::FindComponentByType(NameHash TypeName) const
{
	for (const std::unique_ptr<Component>& NextComponent : m_ComponentList)
	{
		if (NextComponent->Type() == TypeName)
		{
			return NextComponent.get();
		}
	}
	return nullptr;
}

Component* GameObject::FindComponentByType(NameHash TypeName)
{
	return const_cast<Component*>(const_cast<const GameObject*>(this)->FindComponentByType(TypeName));
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

void GameObject::Tick(float DeltaTime)
{
	assert(m_World != nullptr);
	for (std::unique_ptr<Component>& NextComponent : m_ComponentList)
	{
		NextComponent->Tick(DeltaTime);
	}
}

void GameObject::Draw(sf::RenderTarget& RenderTarget) const
{
	assert(m_World != nullptr);
	for (const std::unique_ptr<Component>& NextComponent : m_ComponentList)
	{
		NextComponent->Draw(RenderTarget);
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
