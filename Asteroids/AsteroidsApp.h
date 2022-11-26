#pragma once

#include "Engine/App/GameApp.h"
#include "Engine/Gameplay.h"
#include "SFML/Window/Keyboard.hpp"

class AsteroidsApp : public GameApp
{
public:
	static AsteroidsApp& Get();

protected:
	virtual void StartupLoadAssets() override;
	virtual void StartupInit() override;
	virtual void ShutdownCleanup() override;

private:
	void OnEscapePressed(const sf::Event::KeyEvent& keyEvent);

	World m_World;
};
