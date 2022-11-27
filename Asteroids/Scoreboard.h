#pragma once

#include "Engine/Gameplay/GameObject.h"

class TextComponent;

class Scoreboard : public GameObject
{
public:
	virtual void Init() override;

private:
	void UpdateScoreText(int score);

	TextComponent* m_ScoreText = nullptr;
};
