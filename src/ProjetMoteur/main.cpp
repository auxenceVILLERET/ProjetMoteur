#include <windows.h>
#include <iostream>
#include "main.h"

#include "ECS.h"
#include "Entity.h"
#include "MeshComponent.h"
#include "PositionComponent.h"
#include "VelocityComponent.h"
#include "MovementSystem.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    AllocConsole();

    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    freopen_s(&f, "CONOUT$", "w", stderr);
    freopen_s(&f, "CONIN$", "r", stdin);

    std::cout << "Console affiche !" << std::endl;

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

	ecs.Update(1.0f);

	std::cout << "Entity : " << entity->GetId() << "\n";
	std::cout << "Position after update: (" << position->x << ", " << position->y << ", " << position->z << ")\n";
	std::cout << "Entity2 : " << entity2->GetId() << "\n";
	std::cout << "Position after update: (" << position2->x << ", " << position2->y << ", " << position2->z << ")\n";
	system("pause");
}

