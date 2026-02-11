#include "ObstacleSystem.h"
#include "Engine/ECS/ECS.h"
#include "Engine/ECS/Components/ObstacleComponent.h"

void ObstacleSystem::Update(ECS& ecs, float deltaTime)
{
	for(Entity* entity : ecs.GetEntities())
	{
		ObstacleComponent* obstacle = entity->GetComponent<ObstacleComponent>();
		if(obstacle == nullptr)
			continue;
		if(obstacle->IsActive() == false)
			continue;
		XMFLOAT3 pos = entity->GetPosition();
		if(pos.z <= obstacle->GetKillDistance())
		{
			// Mark the entity for destruction
		}
	}
}
