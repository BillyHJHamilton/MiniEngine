#pragma once

#include "Core.h"
#include "GameObject.h"
#include "GameSystem.h"

class World
{
public:
	GameObject* AddObject(GameObject* NewObject);
	void DestroyObject(GameObject* ObjectToDestroy);

	template<typename SystemType>
	SystemType* GetSystem()
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

	void Tick(float DeltaTime);
	void Draw(sf::RenderTarget& RenderTarget) const;

private:
	std::vector<std::unique_ptr<GameObject>> m_ObjectList;
	std::unordered_map<NameHash, std::unique_ptr<GameSystem>> m_SystemMap;
};
