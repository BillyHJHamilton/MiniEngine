#include "AsteroidsApp.h"

#include "Engine/Gameplay.h"
#include "Asteroids/AsteroidSizes.h"
#include "Asteroids/PlayerShip.h"

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
}

void AsteroidsApp::StartupInit()
{
	m_CurrentWorld = &m_World;

	GameObject* playerShip = m_CurrentWorld->AddObject(new PlayerShip());
	if (playerShip)
	{
		playerShip->SetPosition({320.0f, 240.0f});
	}

	SpawnObject<LargeAsteroid>(m_World, {100.0f, 100.0f});
	SpawnObject<LargeAsteroid>(m_World, {500.0f, 100.0f});

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