#pragma once

#include "Engine/Core.h"
#include "Engine/Gameplay/GameObject.h"
#include "Engine/Gameplay/GameSystem.h"

class World
{
public:
	GameObject* AddObject(GameObject* newObject);

	template<typename ObjectType> ObjectType* AddObject();
	template<typename ObjectType, typename... ArgumentList> ObjectType* EmplaceObject(ArgumentList... args);

	template<typename SystemType> SystemType* GetSystem();

	void SetSize(Vec2 newSize);
	Vec2 GetSize() const;

	void Tick(float deltaTime);
	void Draw(sf::RenderTarget& renderTarget) const;

private:
	void CleanUpDeadObjects();

	std::vector<std::unique_ptr<GameObject>> m_ObjectList;
	std::unordered_map<NameHash, std::unique_ptr<GameSystem>> m_SystemMap;

	Vec2 m_Size = {640.0f, 480.0f};
};

// Template implementations

template<typename ObjectType>
ObjectType* World::AddObject()
{
	return static_cast<ObjectType*>(AddObject(new ObjectType));
}

template<typename ObjectType, typename... ArgumentList>
ObjectType* World::EmplaceObject(ArgumentList... args)
{
	return static_cast<ObjectType*>(AddObject(new ObjectType(std::forward<ArgumentList>(args)...)));
}

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
