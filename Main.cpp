#include <SFML/Graphics.hpp>

#include "Engine/AssetManager.h"
#include "Engine/Core.h"
#include "Engine/Components/CollisionComponent.h"
#include "Engine/Components/MoveComponent.h"
#include "Engine/Components/SpriteComponent.h"
#include "Engine/GameObject.h"
#include "Engine/GameSystem.h"
#include "Engine/World.h"

#include "Engine/Event.h"

#include <iostream>

void LoadAssets()
{
	g_AssetManager.LoadTexture("Fighter1", "Textures/Fighter1.gif");
}

void RunGame();

int main()
{
	#if UNIT_TESTS
		TestEvents();
		NameHash::UnitTest();
		GameObject::UnitTest();
	#endif

	LoadAssets();

	RunGame();

#if DEBUG_MEMORY
	GameObject::CheckMemoryReleased();
	GameSystem::CheckMemoryReleased();
	Component::CheckMemoryReleased();
#endif

	return 0;
}

void RunGame()
{
	World MyWorld;
	GameObject* Ship1 = MyWorld.AddObject(new GameObject());
	Ship1->AddComponent(new SpriteComponent("Fighter1"));
	Ship1->AddComponent(new MoveComponent());
	Ship1->AddComponent(new CollisionComponent());
	Ship1->SetPosition({200.0f,50.0f});

	MoveComponent* Ship1Move = Ship1->FindComponent<MoveComponent>();
	if (Ship1Move != nullptr)
	{
		Ship1Move->SetAcceleration({0,1});
	}

	struct MyTempObject
	{
		int i = 0;
		void Print(GameObject* a, CollisionComponent* b)
		{
			std::cout << "Hit obstacle (" << i++ << ")" << std::endl;
		}
	} temp;
	CollisionComponent* Ship1Collision = Ship1->FindComponent<CollisionComponent>();
	Ship1Collision->SetCollisionEventForLayer(NameHash("Obstacle"), &temp, &MyTempObject::Print);
	Ship1Collision->SetCollider(new CircleCollider(25.0f));

	GameObject* Ship2 = MyWorld.AddObject(new GameObject());
	Ship2->AddComponent(new SpriteComponent("Fighter1"));
	Ship2->AddComponent(new CollisionComponent());
	Ship2->SetPosition({200.0f,100.0f});

	CollisionComponent* Ship2Collision = Ship2->FindComponent<CollisionComponent>();
	Ship2Collision->AddToLayer("Obstacle");
	Ship2Collision->SetCollider(new CircleCollider(25.0f));

	sf::RenderWindow window(sf::VideoMode(400, 400), "MiniEngine");

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		MyWorld.Draw(window);
		MyWorld.Tick(0.1f);

		window.display();
	}
}
