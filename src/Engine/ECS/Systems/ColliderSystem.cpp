#include "ColliderSystem.h"
#include "Engine/ECS/ECS.h"
#include "Engine/ECS/Entity.h"

void ColliderSystem::Update(ECS& ecs, float deltaTime)
{
	for (Entity* entity : ecs.GetEntities())
	{

	}
}

bool ColliderSystem::CheckSphereCollision(Entity* entityA, Entity* entityB)
{
	return false; 
}

