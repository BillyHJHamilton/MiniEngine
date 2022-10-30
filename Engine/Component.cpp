#include "Component.h"

#include "GameObject.h"

#if DEBUG_MEMORY
int Component::s_NumCreated = 0;
int Component::s_NumDestroyed = 0;

void Component::CheckMemoryReleased()
{
	if (s_NumCreated == s_NumDestroyed)
	{
		std::cout << "Component Memory OK." << std::endl;
	}
	else
	{
		std::cerr << "Component Memory Error!  Created " << s_NumCreated
			<< ", Destroyed " << s_NumDestroyed << std::endl;
	}
}
#endif

const World* Component::GetWorld() const
{
	assert(GetOwner());
	return GetOwner()->GetWorld();
}

World* Component::GetWorld()
{
	assert(GetOwner());
	return GetOwner()->GetWorld();
}

void Component::SetOwner(GameObject* newOwner)
{
	m_Owner = newOwner;
	OnAddedToObject(newOwner);
}
