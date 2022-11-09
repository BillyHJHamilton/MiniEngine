#include "AssetManager.h"
#include "SFML\Graphics\Texture.hpp"

AssetManager g_AssetManager;

void AssetManager::LoadTexture(NameHash textureName, std::string filename,
	TextureLoadOptions options)
{
	if (m_TextureMap.find(textureName) != m_TextureMap.end())
	{
		#if DEBUG_NAME_HASHING
			std::cerr << "Error: Can't load texture to " << textureName.GetNameString()
				<< " because that name is already in use." << std::endl;
		#endif

		return;
	}

	m_TextureMap.emplace(textureName, sf::Texture());

	// Load the image from a file.
	sf::Image tempImage;
	bool const bLoaded = tempImage.loadFromFile(filename);
	if (!bLoaded)
	{
		m_TextureMap.erase(textureName);
		return;
	}

	// If a transparent colour was given, mask out those pixels.
	if (options.m_TransparentColour != sf::Color::Transparent)
	{
		tempImage.createMaskFromColor(options.m_TransparentColour, 0u);
	}

	// Create texture from the loaded image.
	m_TextureMap[textureName].loadFromImage(tempImage);
}

const sf::Texture* AssetManager::FindTexture(NameHash textureName)
{
	auto Itr = m_TextureMap.find(textureName);
	if (Itr == m_TextureMap.end())
	{
		return nullptr;
	}
	else
	{
		return &Itr->second;
	}
}
