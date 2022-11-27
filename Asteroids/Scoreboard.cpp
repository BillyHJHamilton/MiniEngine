#include "Scoreboard.h"

#include "Engine/Gameplay/Components/TextComponent.h"

void Scoreboard::Init()
{
	m_ScoreText = AddComponent<TextComponent>();
	if (m_ScoreText)
	{
		m_ScoreText->SetFont("ScoreFont");
		m_ScoreText->m_Text.setFillColor(sf::Color::Red);
		m_ScoreText->m_Text.setString("ASTEROIDS");
		m_ScoreText->m_Text.setCharacterSize(32);
	}
}
