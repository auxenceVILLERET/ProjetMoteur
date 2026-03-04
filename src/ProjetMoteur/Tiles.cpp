#include "ProjetMoteur/Tiles/Tiles.h"
#include "Engine/ECS/Entity.h"
#include "Engine/ResourceManager.h"
#include "Engine/ECS/Components/MeshRendererComponent.h"
#include <cmath>
#include "Engine/ECS/ECS.h"
#include "Engine/ECS/Components/EnemyComponent.h"
#include "Core/Random.h"
#include <iostream>
#include "Engine/ECS/Components/ObstacleComponent.h"


void Tiles::MoveZ(float dist)
{
	m_position.z += dist;
}

void Tiles::CreateArc(ECS* ecs, Renderer* renderer, XMFLOAT3 center, float radius, float startAngle, float endAngle, int segments)
{
	float angleStep = (endAngle - startAngle) / segments;
	for (int i = 0; i <= segments; ++i)
	{
		float angle = startAngle + i * angleStep;
		float x = center.x + radius * cosf(angle);
		float z = center.z + radius * sinf(angle);

		XMFLOAT3 offset = { x , 0.0f, z  };

		Entity* tileEntity = ecs->CreateEntity<Entity>();
		tileEntity->AddComponent<MeshRendererComponent>()->SetMesh(ResourceManager::Instance().GetMeshPreset(MeshPreset::Cylinder), renderer);
		tileEntity->SetPosition(GetPosition().x + offset.x, GetPosition().y + offset.y, GetPosition().z + offset.z);
		tileEntity->SetScale({ 0.1f, 0.3f, 0.1f });
		tileEntity->RotateX(XM_PIDIV2);

		GetEntities().push_back(tileEntity);
		GetLocalOffset().push_back(offset);
	}
}

void Tiles::SpawnEnemy()
{
	Random random;
	int choosenEnemy = random.RandomInt(1, 3);
	float chance = random.Chance(50.0f);

	if(chance < 0.5f)
	{
		choosenEnemy = 0; // no enemy
	}


	for(Entity* entity : m_entities)
	{
		if (entity->GetComponent<EnemyComponent>())
		{
			if (entity->GetComponent<EnemyComponent>()->GetID() == choosenEnemy)
			{
				entity->SetActive(true);
				m_activeEnemy = entity;
			}
			else
			{
				entity->SetActive(false);
			}
		}
		if(entity->GetComponent<ObstacleComponent>())
		{
			if (choosenEnemy == 0)
			{
				entity->SetActive(false);
			}
			else
			{
				entity->SetActive(true);
			}
		}
	}
}

void Tiles::SetActive(bool active)
{
	m_active = active;

	for (Entity* entity : m_entities)
	{
		if (entity->GetComponent<EnemyComponent>()) continue;
		if (entity->GetComponent<ObstacleComponent>()) continue;
		entity->SetActive(active);
	}
}

void Tiles::SetActiveEnemies(bool active)
{
	for (Entity* entity : m_entities)
	{
		if (entity->GetComponent<EnemyComponent>())
		{
			entity->SetActive(active);
		}
		if(entity->GetComponent<ObstacleComponent>())
		{
			entity->SetActive(active);
		}
	}
}
