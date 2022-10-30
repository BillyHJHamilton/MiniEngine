#pragma once

#if _DEBUG
	#define UNIT_TESTS true
	#define DEBUG_NAME_HASHING true
	#define DEBUG_MEMORY true
#else
	#define UNIT_TESTS true // For now, I'd like to test release, too.
	#define DEBUG_NAME_HASHING false
	#define DEBUG_MEMORY false
#endif

#include <assert.h>
#include <iostream>
#include <unordered_map>
#include <memory>
#include <string>
#include <vector>

#include <SFML/System/Vector2.hpp>

#include "NameHash.h"
#include "Event.h"

class Component;
class GameObject;
class GameSystem;
class World;

namespace sf
{
	class RenderTarget;
	class Sprite;
	class Texture;
}

using Vec2 = sf::Vector2f;
