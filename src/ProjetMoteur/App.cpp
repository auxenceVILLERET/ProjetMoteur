#include "App.h"

#include <iostream>
#include "ECS/ECS.h"
#include "Engine/ECS/MovementSystem.h"
#include "Engine/ECS/PositionComponent.h"
#include "Engine/ECS/VelocityComponent.h"

App::App(Engine& engine) : m_engine(engine)
{
	engine.SetInitCallback(std::bind(&App::Initialize, this));
	engine.SetUpdateCallback(std::bind(&App::Update, this));
	engine.SetShutdownCallback(std::bind(&App::Shutdown, this));

}

void App::Initialize()
{
	// Initialize your application here
}

void App::Update()
{
	ECS ecs;

	Entity* entity = ecs.CreateEntity<Entity>();
	PositionComponent* position = entity->AddComponent<PositionComponent>();
	VelocityComponent* velocity = entity->AddComponent<VelocityComponent>();
	position->x = 5.0f;
	position->y = 10.0f;
	position->z = 0.0f;
	velocity->vx = 1.0f;
	velocity->vy = 0.0f;
	velocity->vz = 5.0f;

	Entity* entity2 = ecs.CreateEntity<Entity>();
	PositionComponent* position2 = entity2->AddComponent<PositionComponent>();
	VelocityComponent* velocity2 = entity2->AddComponent<VelocityComponent>();
	position2->x = -5.0f;
	position2->y = 0.0f;
	position2->z = 3.0f;
	velocity2->vx = 0.0f;
	velocity2->vy = 2.0f;
	velocity2->vz = -1.0f;

	ecs.AddSystem<MovementSystem>();

	std::cout << "Entity : " << entity->GetId() << "\n";
	std::cout << "Position after update: (" << position->x << ", " << position->y << ", " << position->z << ")\n";
	std::cout << "Entity2 : " << entity2->GetId() << "\n";
	std::cout << "Position after update: (" << position2->x << ", " << position2->y << ", " << position2->z << ")\n";

}

void App::Shutdown()
{
	// Clean up your application here
}
