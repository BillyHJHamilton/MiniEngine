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
	// Copy the list before iteration as objects may be created during ticking.
	// Only objects that existed at the start of the tick will be ticked.
	std::vector<GameObject*> objectsToTick;
	objectsToTick.reserve(m_ObjectList.size());
	for (std::unique_ptr<GameObject>& object : m_ObjectList)
	{
		objectsToTick.push_back(object.get());
	}

	bool foundInvalid = false;
	for (GameObject* object : objectsToTick)
	{
		if (IsValid(object))
		{
			object->Tick(deltaTime);
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

	// Clean up dead objects
	CoreUtility::RemoveSwapAllInvalidItems(m_ObjectList);
}

void World::Draw(sf::RenderTarget& renderTarget) const
{
	for (const std::unique_ptr<GameObject>& NextObject : m_ObjectList)
	{
		if (IsValid(NextObject.get()))
		{
			NextObject->Draw(renderTarget);
		}
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
