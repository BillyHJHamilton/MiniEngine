#pragma once

#include "ScoreSystem.h"

void ScoreSystem::ResetScore()
{
	m_Score = 0;
	EventScoreUpdated.Broadcast(m_Score);
}

void ScoreSystem::SetScore(int newScore)
{
	m_Score = newScore;
	EventScoreUpdated.Broadcast(m_Score);
}

void ScoreSystem::AddScore(int scoreToAdd)
{
	m_Score += scoreToAdd;
	EventScoreUpdated.Broadcast(m_Score);
}

int ScoreSystem::GetScore()
{
	return m_Score;
}
