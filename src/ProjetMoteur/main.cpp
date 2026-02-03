#include <iostream>
#include "main.h"

#include "ECS.h"
#include "Entity.h"
#include "MeshComponent.h"
#include "PositionComponent.h"
#include "VelocityComponent.h"
#include "MovementSystem.h"

int main()
{
	ECS ecs;

	Entity* entity = ecs.CreateEntity<Entity>();
	PositionComponent* position = ecs.AddComponent<PositionComponent>(entity);
	VelocityComponent* velocity = ecs.AddComponent<VelocityComponent>(entity);
	position->x = 5.0f;
	position->y = 10.0f;
	position->z = 0.0f;
	velocity->vx = 1.0f;
	velocity->vy = 0.0f;
	velocity->vz = 5.0f;

	Entity* entity2 = ecs.CreateEntity<Entity>();
	PositionComponent* position2 = ecs.AddComponent<PositionComponent>(entity2);
	VelocityComponent* velocity2 = ecs.AddComponent<VelocityComponent>(entity2);
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
}