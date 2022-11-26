#include "GameApp.h"

#include "Engine/Gameplay.h"
#include "SFML/Window/Event.hpp"

GameApp* GameApp::s_Instance = nullptr;

GameApp::~GameApp()
{
#if DEBUG_MEMORY
	DestroyCheckMemory();
#endif
}

GameApp& GameApp::Get()
{
	assert(s_Instance != nullptr);
	return *s_Instance;
}

AssetManager& GameApp::GetAssetManager()
{
	assert(s_Instance != nullptr);
	return Get().m_AssetManager;
}

InputEventManager& GameApp::GetInputEventManager()
{
	assert(s_Instance != nullptr);
	return Get().m_InputEventManager;
}

void GameApp::Run()
{
	assert(s_Instance == nullptr);
	s_Instance = this;

#if UNIT_TESTS
	StartupEngineTests();
#endif UNIT_TESTS

	StartupLoadAssets();
	StartupInit();

	CreateWindow();

	AppLoop();

	ShutdownCleanup();
}

void GameApp::CreateWindow()
{
	// TODO Provide a way to configure the window.
	m_MainWindow.create(sf::VideoMode(640, 480), "MiniEngine");

	m_Clock.restart();
}

void GameApp::AppLoop()
{
	float frameStart = m_Clock.getElapsedTime().asSeconds();
	while (m_MainWindow.isOpen())
	{
		float fixedFrameDuration = 1.0f / m_FixedFPS;
		float frameEnd = frameStart + fixedFrameDuration;

		AppHandleEvents();

		AppTick(fixedFrameDuration);

		do
		{
			AppDraw();
		} while (m_Clock.getElapsedTime().asSeconds() < frameEnd);

		frameStart = frameEnd;
	}
}

void GameApp::AppHandleEvents()
{
	sf::Event nextEvent;
	while (m_MainWindow.pollEvent(nextEvent))
	{
		switch (nextEvent.type)
		{
			case sf::Event::Closed:
				m_MainWindow.close();
				break;

			case sf::Event::KeyPressed:
				m_InputEventManager.HandleKeyPressed(nextEvent.key);
				break;

			case sf::Event::KeyReleased:
				m_InputEventManager.HandleKeyReleased(nextEvent.key);
				break;
		}
	}
}

void GameApp::AppTick(float deltaTime)
{
	if (m_CurrentWorld != nullptr)
	{
		m_CurrentWorld->Tick(deltaTime);
	}
}

void GameApp::AppDraw()
{
	m_MainWindow.clear();
	if (m_CurrentWorld != nullptr)
	{
		m_CurrentWorld->Draw(m_MainWindow);
	}
	m_MainWindow.display();
}

#if UNIT_TESTS
void GameApp::StartupEngineTests()
{
	TestEvents();
	TestReferences();
	NameHash::UnitTest();
	GameObject::UnitTest();
}
#endif

#if DEBUG_MEMORY
void GameApp::DestroyCheckMemory()
{
	GameObject::CheckMemoryReleased();
	GameSystem::CheckMemoryReleased();
	Component::CheckMemoryReleased();
}
#endif
