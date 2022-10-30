#include "AssetManager.h"
#include "SFML\Graphics\Texture.hpp"

AssetManager g_AssetManager;

void AssetManager::LoadTexture(NameHash TextureName, std::string Filename)
{
	if (m_TextureMap.find(TextureName) != m_TextureMap.end())
	{
		#if DEBUG_NAME_HASHING
			std::cerr << "Error: Can't load texture to " << TextureName.GetNameString()
				<< " because that name is already in use." << std::endl;
		#endif

		return;
	}

	m_TextureMap.emplace(TextureName, sf::Texture());
	bool const bLoaded = m_TextureMap[TextureName].loadFromFile(Filename);
	if (!bLoaded)
	{
		m_TextureMap.erase(TextureName);
	}
}

const sf::Texture* AssetManager::FindTexture(NameHash TextureName)
{
	auto Itr = m_TextureMap.find(TextureName);
	if (Itr == m_TextureMap.end())
	{
		return nullptr;
	}
	else
	{
		return &Itr->second;
	}
}
