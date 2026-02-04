#pragma once
#include "System.h"
#include "ECS.h"
#include "PositionComponent.h"
#include "VelocityComponent.h"

class MovementSystem : public System
{
public:
	void Update(ECS& ecs, float deltaTime) override
	{
		for (Entity* entity : ecs.GetEntities())
		{
			PositionComponent* position = entity->GetComponent<PositionComponent>();
			VelocityComponent* velocity = entity->GetComponent<VelocityComponent>();
			if (position && velocity)
			{
				position->x += velocity->vx * deltaTime;
				position->y += velocity->vy * deltaTime;
				position->z += velocity->vz * deltaTime;
			}
		}
	}
};

