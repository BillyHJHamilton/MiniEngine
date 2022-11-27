#include "AsteroidsApp.h"

#include "Engine/Gameplay.h"
#include "Asteroids/AsteroidSizes.h"
#include "Asteroids/PlayerShip.h"
#include "Asteroids/Scoreboard.h"

AsteroidsApp& AsteroidsApp::Get()
{
	return static_cast<AsteroidsApp&>(GameApp::Get());
}

void AsteroidsApp::StartupLoadAssets()
{
	m_AssetManager.LoadTexture("Fighter1", "Asteroids/Textures/Fighter1.gif",
		AssetManager::TextureLoadOptions(sf::Color::Green));
	m_AssetManager.LoadTexture("Asteroid", "Asteroids/Textures/AsteroidStrip.png",
		AssetManager::TextureLoadOptions(sf::Color::White));
	m_AssetManager.LoadTexture("Explode64", "Asteroids/Textures/Explode64.png");
	m_AssetManager.LoadTexture("BlueLaser", "Asteroids/Textures/BlueLaser.png",
		AssetManager::TextureLoadOptions(sf::Color::Black));

	m_AssetManager.LoadFont("ScoreFont", "Asteroids/Fonts/FSEX302.ttf");
}

void AsteroidsApp::StartupInit()
{
	m_CurrentWorld = &m_World;

	SpawnObject<PlayerShip>(m_World, {320.0f, 240.0f});

	SpawnObject<LargeAsteroid>(m_World, {100.0f, 100.0f});
	SpawnObject<LargeAsteroid>(m_World, {500.0f, 100.0f});

	SpawnObject<Scoreboard>(m_World, {25.0f, 11.0f});

	m_InputEventManager.GetKeyPressedEvent(sf::Keyboard::Escape).AddDelegate(
		this, &AsteroidsApp::OnEscapePressed);
}

void AsteroidsApp::ShutdownCleanup()
{
}

void AsteroidsApp::OnEscapePressed(const sf::Event::KeyEvent& keyEvent)
{
	m_MainWindow.close();
}