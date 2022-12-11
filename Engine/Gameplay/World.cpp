#include "World.h"

void World::SetSize(Vec2 newSize)
{
	m_Size = newSize;
}

Vec2 World::GetSize() const
{
	return m_Size;
}

void World::Tick(float deltaTime)
{
	// Tick game objects.
	// Iterate by index because new objects could be added during the tick.
	for (int i = 0; i < m_ObjectList.size(); ++i)
	{
		if (IsValid(m_ObjectList[i].get()))
		{
			m_ObjectList[i]->Tick(deltaTime);
		}
	}

	// Tick game systems.
	std::vector<GameSystem*> systemsToTick;
	systemsToTick.reserve(m_SystemMap.size());
	for (std::pair<const NameHash, std::unique_ptr<GameSystem>>& system : m_SystemMap)
	{
		if (system.second)
		{
			systemsToTick.push_back(system.second.get());
		}
	}
	for (GameSystem* system : systemsToTick)
	{
		system->Tick(deltaTime);
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
			m_ObjectList[i] = std::move(m_ObjectList.back());
			m_ObjectList.pop_back();
			--i;
		}
	}
}
