#pragma once

#include "Engine/Core.h"
#include "Engine/Gameplay/GameObject.h"
#include "Engine/Gameplay/GameSystem.h"

class World
{
public:
	GameObject* AddObject(GameObject* newObject);
	void DestroyObject(GameObject* objectToDestroy);
	
	template<typename SystemType> SystemType* GetSystem();

	void Tick(float deltaTime);
	void Draw(sf::RenderTarget& renderTarget) const;

private:
	std::vector<std::unique_ptr<GameObject>> m_ObjectList;
	std::unordered_map<NameHash, std::unique_ptr<GameSystem>> m_SystemMap;
};

// Template implementations

template<typename SystemType>
SystemType* World::GetSystem()
{
	NameHash systemName = SystemType::StaticType();
	auto systemItr = m_SystemMap.find(systemName);
	if (systemItr == m_SystemMap.end())
	{
		auto result = m_SystemMap.emplace(systemName, new SystemType);
		if (!result.second) // insert failed
		{
			return nullptr;
		}
		systemItr = result.first;
	}
	return static_cast<SystemType*>(systemItr->second.get());
}
