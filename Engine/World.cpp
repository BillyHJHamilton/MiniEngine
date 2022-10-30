#include "World.h"

#include "GameObject.h"

void World::Tick(float DeltaTime)
{
	for (std::unique_ptr<GameObject>& NextObject : m_ObjectList)
	{
		NextObject->Tick(DeltaTime);
	}
}

void World::Draw(sf::RenderTarget& RenderTarget) const
{
	for (const std::unique_ptr<GameObject>& NextObject : m_ObjectList)
	{
		NextObject->Draw(RenderTarget);
	}
}

GameObject* World::AddObject(GameObject* NewObject)
{
	assert(NewObject != nullptr);
	m_ObjectList.emplace_back(NewObject);
	NewObject->SetWorld(this);
	return NewObject;
}

void World::DestroyObject(GameObject* ObjectToDestroy)
{
	auto Itr = std::find_if(m_ObjectList.begin(), m_ObjectList.end(),
		[ObjectToDestroy](std::unique_ptr<GameObject>& NextObject)
		{
			return NextObject.get() == ObjectToDestroy;
		}
	);
	
	// If the object was found, swap it to the back of the list, then pop it off.
	if (Itr != m_ObjectList.end())
	{
		auto BackItr = m_ObjectList.end() - 1;
		if (Itr != BackItr)
		{
			std::iter_swap(Itr, BackItr);
		}
		m_ObjectList.pop_back();
	}
}
