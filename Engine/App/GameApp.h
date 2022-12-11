#pragma once

#include "Engine/Core.h"

#include "Engine/App/AssetManager.h"
#include "Engine/App/InputEventManager.h"
#include "SFML/Graphics/RenderWindow.hpp"

class World;

class GameApp
{
public:
	virtual ~GameApp();

	static GameApp& Get();
	static AssetManager& GetAssetManager();
	static InputEventManager& GetInputEventManager();

	// Returns seconds since the game app started.
	double GetClockTime() const;

	void Run();

protected:
	// Override this to load assets at startup.
	virtual void StartupLoadAssets() {}

	// Override this to initialize systems at app startup.
	virtual void StartupInit();

	virtual void CreateWindow();

	virtual void AppLoop();

	void AppHandleEvents();
	void AppTick(float deltaTime);
	void AppDraw();

	// Override this to clean up systems at app shutdown.
	virtual void ShutdownCleanup() {}

	sf::RenderWindow m_MainWindow;
	sf::Clock m_Clock;

	float m_FixedFPS = 60.0f;
	float m_LastTick = 0.0f;

	AssetManager m_AssetManager;
	InputEventManager m_InputEventManager;
	World* m_CurrentWorld = nullptr;

private:
#if UNIT_TESTS
	void StartupEngineTests();
#endif

#if DEBUG_MEMORY
	void DestroyCheckMemory();
#endif

	static GameApp* s_Instance;
};
