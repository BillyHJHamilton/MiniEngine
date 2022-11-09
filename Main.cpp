#include <SFML/Graphics.hpp>

#include "Engine/AssetManager.h"
#include "Engine/Gameplay.h"
#include "Engine/Gameplay/Components/CollisionComponent.h"
#include "Engine/Gameplay/Components/MoveComponent.h"
#include "Engine/Gameplay/Components/SpriteComponent.h"
#include "Engine/Gameplay/Components/WrapAroundComponent.h"

void LoadAssets()
{
	g_AssetManager.LoadTexture("Fighter1", "Textures/Fighter1.gif",
		AssetManager::TextureLoadOptions(sf::Color::Green));
	g_AssetManager.LoadTexture("Asteroid", "Textures/AsteroidStrip.png",
		AssetManager::TextureLoadOptions(sf::Color::White));
}

void RunGame();

int main()
{
	#if UNIT_TESTS
		TestEvents();
		TestReferences();
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
	Ship1->AddComponent(new CollisionComponent());
	Ship1->AddComponent(new MoveComponent());
	Ship1->AddComponent(new SpriteComponent("Fighter1"));
	Ship1->AddComponent(new WrapAroundComponent());
	Ship1->SetPosition({200.0f,50.0f});

	WeakRef<GameObject> wShip1 = Ship1->GetWeakRef();

	MoveComponent* Ship1Move = Ship1->FindComponent<MoveComponent>();
	if (Ship1Move != nullptr)
	{
		Ship1Move->SetVelocity({0.,25.0f});
		//Ship1Move->SetAcceleration({0,0.4f});
	}

	WeakRef<Component> wCompA = ComponentWeakRef(Ship1->FindComponentByType(MoveComponent::StaticType()));
	WeakRef<MoveComponent> wMoveA = ComponentWeakRef(Ship1Move);
	assert(wCompA == wMoveA);

	WeakRef<Component> wCompB = Ship1->FindComponentWeakRefByType(MoveComponent::StaticType());
	WeakRef<MoveComponent> wMoveB = Ship1->FindComponentWeakRef<MoveComponent>();
	assert(wCompB == wCompA);
	assert(wCompB == wMoveB);
	assert(wCompB == wMoveA);

	Ship1->FindComponent<WrapAroundComponent>()->m_BoundSize = {400.0f, 400.0f};

	SpriteComponent* Ship1Sprite = Ship1->FindComponent<SpriteComponent>();
	if (Ship1Sprite != nullptr)
	{
		Ship1Sprite->m_Sprite.setRotation(45.0f);
		Ship1Sprite->m_Sprite.setOrigin({16.0f, 16.0f});
	}

	struct MyTempObject
	{
		bool printed = false;
		void Print(GameObject* a, CollisionComponent* b)
		{
			if (!printed)
			{
				//printed = true;
				std::cout << "Hit obstacle" << std::endl;
			}
		}
	} temp;
	CollisionComponent* Ship1Collision = Ship1->FindComponent<CollisionComponent>();
	Ship1Collision->SetCollisionEventForLayer(NameHash("Obstacle"), &temp, &MyTempObject::Print);
	Ship1Collision->SetCollider(new CircleCollider(16.0f));
	Ship1Collision->m_DebugDraw = true;

	GameObject* asteroid = MyWorld.AddObject(new GameObject());
	asteroid->AddComponent(new SpriteComponent("Asteroid"));
	asteroid->AddComponent(new CollisionComponent());
	asteroid->SetPosition({100.0f,300.0f});

	CollisionComponent* asteroidCollision = asteroid->FindComponent<CollisionComponent>();
	asteroidCollision->AddToLayer("Obstacle");
	asteroidCollision->SetCollider(new CircleCollider(26.0f));
	//asteroidCollision->SetCollider(new AlignedBoxCollider({32.0f, 32.0f}));
	asteroidCollision->m_DebugDraw = true;

	SpriteComponent* asteroidSprite = asteroid->FindComponent<SpriteComponent>();
	asteroidSprite->SetupSubimages({64,64}, {0,0}, 50, 50, 0);
	asteroidSprite->m_Sprite.setOrigin(32,32);
	asteroidSprite->Animate(SpriteComponent::AnimationMode::SubimagesPerSecond, 20.0f, true);

	GameObject* point1 = MyWorld.AddObject(new GameObject());
	point1->AddComponent(new CollisionComponent());
	point1->SetPosition({200.0f,200.0f});

	Component* point1CollisionBase = point1->FindComponentByType(CollisionComponent::StaticType());
	CollisionComponent* point1Collision = Downcast<CollisionComponent>(point1CollisionBase);
	assert(point1Collision != nullptr);
	point1Collision ->AddToLayer("Obstacle");
	point1Collision ->SetCollider(new PointCollider());
	point1Collision ->m_DebugDraw = true;

	SpriteComponent* invalidSpriteComponent = Downcast<SpriteComponent>(point1CollisionBase);
	assert(invalidSpriteComponent == nullptr);

	sf::RenderWindow window(sf::VideoMode(400, 400), "MiniEngine");
	sf::Clock clock;

	while (window.isOpen())
	{
		float deltaTime = clock.getElapsedTime().asSeconds();
		clock.restart();
		MyWorld.Tick(deltaTime);

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		MyWorld.Draw(window);
		Ship1Sprite->m_Sprite.setRotation(Ship1Sprite->m_Sprite.getRotation() + 1.0f);

		window.display();

	}
}
