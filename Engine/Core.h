#pragma once

// Includes platform #defines and essential standard library headers.
#include "Core/CoreHeader.h"

#include <SFML/System/Vector2.hpp>

#include "Core/CoreUtility.h"
#include "Core/Event.h"
#include "Core/NameHash.h"
#include "Core/Reference.h"
#include "Core/TypeInfo.h"

class Component;
class GameObject;
class GameSystem;
class World;

namespace sf
{
	class RenderStates;
	class RenderTarget;
	class Sprite;
	class Texture;
}

using Vec2 = sf::Vector2f;
using IntVec2 = sf::Vector2i;
