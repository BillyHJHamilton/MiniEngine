#include "Scoreboard.h"

#include "Asteroids/ScoreSystem.h"
#include "Engine/Gameplay/World.h"
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

	// Get the score system and bind to its update event.
	ScoreSystem* scoreSystem = GetWorld()->GetSystem<ScoreSystem>();
	if (scoreSystem)
	{
		scoreSystem->EventScoreUpdated.AddDelegate(this, &Scoreboard::UpdateScoreText);
		UpdateScoreText(scoreSystem->GetScore());
	}
}

void Scoreboard::UpdateScoreText(int score)
{
	if (m_ScoreText)
	{
		m_ScoreText->m_Text.setString("Score: " + std::to_string(score));
	}
}
