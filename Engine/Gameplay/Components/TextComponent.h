#pragma once

#include "Engine/Core.h"
#include "Engine/Gameplay/Component.h"

#include "SFML/Graphics/Text.hpp"

class TextComponent : public Component
{
public:
	MACRO_DeclareTypeInfo(TextComponent)

	void SetVisible(bool visibility) { m_Visible = visibility; };

	void SetFont(NameHash fontName);

	virtual void Draw(sf::RenderTarget& renderTarget, const sf::RenderStates& renderStates) const override;

	// Public to allow changing local transform, colour, size, etc.
	sf::Text m_Text;

private:
	bool m_Visible = true;
};
