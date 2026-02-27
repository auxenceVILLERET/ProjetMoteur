#include "ProjetMoteur/Tiles/Tiles.h"
#include "Engine/ECS/Entity.h"
#include "Engine/RessourceManager.h"
#include "Engine/ECS/Components/MeshRendererComponent.h"
#include <cmath>
#include "Engine/ECS/ECS.h"

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
		tileEntity->AddComponent<MeshRendererComponent>()->SetMesh(RessourceManager::Instance().GetCylinder(), renderer);
		tileEntity->SetPosition(GetPosition().x + offset.x, GetPosition().y + offset.y, GetPosition().z + offset.z);
		tileEntity->SetScale({ 0.1f, 0.3f, 0.1f });
		tileEntity->RotateX(XM_PIDIV2);

		GetEntities().push_back(tileEntity);
		GetLocalOffset().push_back(offset);
	}
}

void Tiles::SetActive(bool active)
{
	m_active = active;

	for (Entity* entity : m_entities)
	{
		entity->SetActive(active);
	}
}