#pragma once

#include "Engine/Gameplay/GameSystem.h"

class ScoreSystem : public GameSystem
{
public:
	void ResetScore();
	void SetScore(int newScore);
	void AddScore(int scoreToAdd);
	int GetScore();
	
	MulticastEvent<int> EventScoreUpdated;

private:
	int m_Score = 0;
};
