#include "World.h"

void World::Tick(float deltaTime)
{
	// Iterate by index because new objects could be added during the tick.
	for (int i = 0; i < m_ObjectList.size(); ++i)
	{
		if (IsValid(m_ObjectList[i].get()))
		{
			m_ObjectList[i]->Tick(deltaTime);
		}
	}

	CleanUpDeadObjects();
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
	newObject->Init();
	return newObject;
}

void World::CleanUpDeadObjects()
{
	for (int i = 0; i < m_ObjectList.size(); ++i)
	{
		if (!m_ObjectList[i]->IsValid())
		{
			std::swap(m_ObjectList[i], m_ObjectList.back());
			m_ObjectList.pop_back();
			--i;
		}
	}
}
