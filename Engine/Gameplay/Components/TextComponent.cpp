#include "TextComponent.h"

#include "Engine/App/GameApp.h"
//#include "Engine/Gameplay/GameObject.h"
#include "SFML/Graphics/RenderTarget.hpp"

TypeInfo const TextComponent::s_TypeInfo = TypeInfo("TextComponent", {"Component"});

void TextComponent::SetFont(NameHash fontName)
{
	const sf::Font* textFont = GameApp::GetAssetManager().FindFont(fontName);
	if (textFont != nullptr)
	{
		m_Text.setFont(*textFont);
	}
}

void TextComponent::Draw(sf::RenderTarget& RenderTarget, const sf::RenderStates& renderStates) const
{
	if (m_Visible && m_Text.getFont() != nullptr)
	{
		RenderTarget.draw(m_Text, renderStates);
	}
}
