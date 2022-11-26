#pragma once

#include "Engine/Core.h"
#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/Texture.hpp"

namespace sf
{
	class Texture;
}

class AssetManager
{
public:
	struct TextureLoadOptions
	{
		// Pixels that exactly match the provided colour will be changed to transparent.
		sf::Color m_TransparentColour = sf::Color::Transparent;
	};

	void LoadTexture(NameHash textureName, std::string filename,
		TextureLoadOptions options = TextureLoadOptions());

	const sf::Texture* FindTexture(NameHash textureName);
private:
	std::unordered_map<NameHash,sf::Texture> m_TextureMap;
};

//extern AssetManager g_AssetManager;
