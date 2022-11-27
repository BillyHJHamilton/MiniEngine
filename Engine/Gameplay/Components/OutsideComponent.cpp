#include "OutsideComponent.h"

#include "Engine/Gameplay.h"
#include "Engine/Gameplay/Components/SpriteComponent.h"

TypeInfo const OutsideComponent::s_TypeInfo = TypeInfo("OutsideComponent", {"Component"});

void OutsideComponent::SetReferenceSprite(const SpriteComponent* spriteComponent)
{
	if (spriteComponent == nullptr)
	{
		m_ReferenceSprite.Reset();
	}
	else
	{
		m_ReferenceSprite = ComponentWeakRef(spriteComponent);
	}
}

void OutsideComponent::SetResponse(Response response)
{
	m_Response = response;
}

void OutsideComponent::Tick(float deltaTime)
{
	const World* world = GetWorld();
	assert(world);

	sf::FloatRect worldBox({0.0f, 0.0f}, world->GetSize());
	sf::FloatRect objectBox(GetOwner()->GetPosition(), {0.0f,0.0f});
	if (m_ReferenceSprite.IsValid())
	{
		objectBox = m_ReferenceSprite->m_Sprite.getGlobalBounds();
		objectBox.left += GetOwner()->GetPosition().x;
		objectBox.top += GetOwner()->GetPosition().y;
	}

	if (!worldBox.intersects(objectBox))
	{
		TriggerOutside(worldBox, objectBox);
	}
}

void OutsideComponent::TriggerOutside(const sf::FloatRect& worldBox, const sf::FloatRect& objectBox)
{
	EventOutside.Broadcast(GetOwner());

	if (m_Response == Response::Destroy)
	{
		GetOwner()->Destroy();
	}
	else if (m_Response == Response::Wrap)
	{
		OutsideWrap(worldBox, objectBox);
	}
}

void OutsideComponent::OutsideWrap(const sf::FloatRect& worldBox, const sf::FloatRect& objectBox)
{
	Vec2 newPosition = GetOwner()->GetPosition();

	// Wrap off right side
	if (objectBox.left > worldBox.width)
	{
		newPosition.x = std::fmod(GetOwner()->GetPosition().x, worldBox.width) - objectBox.width;
	}

	// Wrap off left side
	else if (objectBox.left + objectBox.width < 0)
	{
		newPosition.x = worldBox.width + std::fmod(GetOwner()->GetPosition().x, worldBox.width) + objectBox.width;
	}

	// Wrap off bottom
	if (objectBox.top > worldBox.height)
	{
		newPosition.y = std::fmod(GetOwner()->GetPosition().y, worldBox.height) - objectBox.height;
	}

	// Wrap off top
	else if (objectBox.top + objectBox.height < 0)
	{
		newPosition.y = worldBox.height + std::fmod(GetOwner()->GetPosition().y, worldBox.height) + objectBox.height;
	}

	GetOwner()->SetPosition(newPosition);
}
