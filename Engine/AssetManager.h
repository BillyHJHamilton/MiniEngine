#pragma once

#include "Engine\Core.h"

namespace sf
{
	class Texture;
}

class AssetManager
{
public:
	void LoadTexture(NameHash TextureName, std::string Filename);

	const sf::Texture* FindTexture(NameHash TextureName);
private:
	std::unordered_map<NameHash,sf::Texture> m_TextureMap;
};

extern AssetManager g_AssetManager;
