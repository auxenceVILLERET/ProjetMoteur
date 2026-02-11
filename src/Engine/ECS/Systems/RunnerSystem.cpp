#include "RunnerSystem.h"
#include "Engine/ECS/ECS.h"
#include "Engine/ECS/Components/RunnerComponent.h"
#include "Engine/ECS/Components/ObstacleComponent.h"

void RunnerSystem::Update(ECS& ecs, float deltaTime)
{
	RunnerComponent* runner = GetRunnerComponent(ecs);
	if (runner == nullptr)
		return;

	float speed = runner->GetSpeed();

	for(Entity* entity : ecs.GetEntities())
	{
		ObstacleComponent* obstacle = entity->GetComponent<ObstacleComponent>();

		if(obstacle == nullptr)
			continue;

		if(obstacle->IsActive() == false)
			continue;

		XMFLOAT3 pos = entity->GetPosition();
		pos.z += speed * deltaTime;
		entity->SetPosition(pos.x, pos.y, pos.z);
	}
}

RunnerComponent* RunnerSystem::GetRunnerComponent(ECS& ecs)
{
	for(Entity* entity : ecs.GetEntities())
	{
		RunnerComponent* runner = entity->GetComponent<RunnerComponent>();
		if(runner != nullptr)
			return runner;
	}
	return nullptr;
}