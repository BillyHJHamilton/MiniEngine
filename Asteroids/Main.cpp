
#include "Asteroids/AsteroidsApp.h"

int main()
{
	AsteroidsApp app;
	app.Run();
	return 0;
}

/*void RunGame()
{
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
	point1Collision->AddToLayer("Obstacle");
	point1Collision->SetCollider(new PointCollider());
	point1Collision->m_DebugDraw = true;

	struct TestObjectMover
	{
		TestObjectMover(WeakRef<GameObject> newObject) : m_Object(newObject) {}

		void OnKeyLeft(sf::Event::KeyEvent const& eventData)
		{
			if (m_Object)
			{
				m_Object->MovePosition({-20.0f,0.0f});
			}
		}

		WeakRef<GameObject> m_Object;
	} objectMover(Ship1->GetWeakRef());

	inputManager.GetKeyPressedEvent(sf::Keyboard::Left).AddDelegate(&objectMover, &TestObjectMover::OnKeyLeft);

	SpriteComponent* invalidSpriteComponent = Downcast<SpriteComponent>(point1CollisionBase);
	assert(invalidSpriteComponent == nullptr);

	sf::RenderWindow window(sf::VideoMode(400, 400), "MiniEngine");
	sf::Clock clock;

	while (window.isOpen())
	{
		float deltaTime = clock.getElapsedTime().asSeconds();
		clock.restart();
		MyWorld.Tick(deltaTime);

		sf::Event nextEvent;
		while (window.pollEvent(nextEvent))
		{
			if (nextEvent.type == sf::Event::Closed)
			{
				window.close();
			}
			else if (nextEvent.type == sf::Event::KeyPressed)
			{
				inputManager.HandleKeyPressed(nextEvent.key);
			}
			else if (nextEvent.type == sf::Event::KeyReleased)
			{
				inputManager.HandleKeyReleased(nextEvent.key);
			}
		}

		window.clear();
		MyWorld.Draw(window);
		Ship1Sprite->m_Sprite.setRotation(Ship1Sprite->m_Sprite.getRotation() + 1.0f);

		window.display();

	}
}
*/
