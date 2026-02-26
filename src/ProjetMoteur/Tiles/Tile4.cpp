#include "Tile4.h"
#include "Engine/ECS/ECS.h"
#include "Render/Renderer.h"
#include "Engine/ECS/Entity.h"
#include "Engine/RessourceManager.h"
#include "Engine/ECS/Components/MeshRendererComponent.h"

void Tile4::Initialize(ECS* ecs, Renderer* renderer)
{
	Entity* tileEntity = ecs->CreateEntity<Entity>();
	tileEntity->AddComponent<MeshRendererComponent>()->SetMesh(RessourceManager::Instance().GetCylinder(), renderer);
	XMFLOAT3 offset = { 2.0f, 0.0f, 0.0f };
	tileEntity->SetPosition(GetPosition().x + offset.x, GetPosition().y + offset.y, GetPosition().z + offset.z);
	tileEntity->SetScaleVector({ 0.1f, 3.0f, 0.1f });
	tileEntity->RotateX(XM_PIDIV2);
	GetEntities().push_back(tileEntity);
	GetLocalOffset().push_back(offset);

	Entity* tileEntity2 = ecs->CreateEntity<Entity>();
	tileEntity2->AddComponent<MeshRendererComponent>()->SetMesh(RessourceManager::Instance().GetCylinder(), renderer);
	XMFLOAT3 offset2 = { 0.0f, 0.0f, 6.0f };
	tileEntity2->SetPosition(GetPosition().x + offset2.x, GetPosition().y + offset2.y, GetPosition().z + offset2.z);
	tileEntity2->SetScaleVector({ 0.1f, 3.0f, 0.1f });
	tileEntity2->RotateX(XM_PIDIV2);
	GetEntities().push_back(tileEntity2);
	GetLocalOffset().push_back(offset2);

	Entity* tileEntity3 = ecs->CreateEntity<Entity>();
	tileEntity3->AddComponent<MeshRendererComponent>()->SetMesh(RessourceManager::Instance().GetCylinder(), renderer);
	XMFLOAT3 offset3 = { 2.0f, 0.0f, 12.0f };
	tileEntity3->SetPosition(GetPosition().x + offset3.x, GetPosition().y + offset3.y, GetPosition().z + offset3.z);
	tileEntity3->SetScaleVector({ 0.1f, 3.0f, 0.1f });
	tileEntity3->RotateX(XM_PIDIV2);
	GetEntities().push_back(tileEntity3);
	GetLocalOffset().push_back(offset3);

	SetActive(false);
	SetIsTurnL(false);
	SetIsTurnR(false);
	SetIn(2);
	SetOut(2);

	if (this->IsActive() == false)
	{
		for (Entity* entity : GetEntities())
		{
			entity->SetActive(false);
		}
	}
}

void Tile4::Update(float deltaTime)
{
	if (this->IsActive() == false)
	{
		for (Entity* entity : GetEntities())
		{
			entity->SetActive(false);
		}
	}
	else
	{
		MoveZ(-(GetSpeed() * deltaTime));

		for (int i = 0; i < GetEntities().size(); i++)
		{
			Entity* entity = GetEntities()[i];
			XMFLOAT3 offset = GetLocalOffset()[i];

			entity->SetActive(true);
			entity->SetPosition(GetPosition().x + offset.x, GetPosition().y + offset.y, GetPosition().z + offset.z);
		}
	}

}
