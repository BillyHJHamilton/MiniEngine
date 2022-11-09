#include "World.h"

void World::Tick(float deltaTime)
{
	for (std::unique_ptr<GameObject>& NextObject : m_ObjectList)
	{
		NextObject->Tick(deltaTime);
	}
}

void World::Draw(sf::RenderTarget& renderTarget) const
{
	for (const std::unique_ptr<GameObject>& NextObject : m_ObjectList)
	{
		NextObject->Draw(renderTarget);
	}
}

GameObject* World::AddObject(GameObject* newObject)
{
	assert(newObject != nullptr);
	m_ObjectList.emplace_back(newObject);
	newObject->SetWorld(this);
	return newObject;
}

void World::DestroyObject(GameObject* objectToDestroy)
{
	auto itr = std::find_if(m_ObjectList.begin(), m_ObjectList.end(),
		[objectToDestroy](std::unique_ptr<GameObject>& NextObject)
		{
			return NextObject.get() == objectToDestroy;
		}
	);
	
	// If the object was found, swap it to the back of the list, then pop it off.
	if (itr != m_ObjectList.end())
	{
		auto backItr = m_ObjectList.end() - 1;
		if (itr != backItr)
		{
			std::iter_swap(itr, backItr);
		}
		m_ObjectList.pop_back();
	}
}
