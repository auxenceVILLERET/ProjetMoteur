#include "Tile1.h"
#include "Engine/ECS/Entity.h"
#include "Engine/ResourceManager.h"
#include "Engine/ECS/Components/MeshRendererComponent.h"

void Tile1::Initialize(ECS* ecs, Renderer* renderer)
{
	Entity* tileEntity = ecs->CreateEntity<Entity>();
	tileEntity->AddComponent<MeshRendererComponent>()->SetMesh(ResourceManager::Instance().GetMeshPreset(MeshPreset::Cylinder), renderer);
	XMFLOAT3 offset = { 0.0f, 0.0f, 0.0f };
	tileEntity->SetPosition(GetPosition().x + offset.x, GetPosition().y + offset.y, GetPosition().z + offset.z);
	tileEntity->SetScale({ 0.1f, 3.0f, 0.1f });
	tileEntity->RotateX(XM_PIDIV2);
	GetEntities().push_back(tileEntity);
	GetLocalOffset().push_back(offset);

	Entity* tileEntity2 = ecs->CreateEntity<Entity>();
	tileEntity2->AddComponent<MeshRendererComponent>()->SetMesh(ResourceManager::Instance().GetMeshPreset(MeshPreset::Cylinder), renderer);
	XMFLOAT3 offset2 = { 0.0f, 0.0f, 4.0f };
	tileEntity2->SetPosition(GetPosition().x + offset2.x, GetPosition().y + offset2.y, GetPosition().z + offset2.z);
	tileEntity2->SetScale({ 0.1f, 3.0f, 0.1f });
	tileEntity2->RotateX(XM_PIDIV2);
	GetEntities().push_back(tileEntity2);
	GetLocalOffset().push_back(offset2);

	Entity* tileEntity4 = ecs->CreateEntity<Entity>();
	tileEntity4->AddComponent<MeshRendererComponent>()->SetMesh(ResourceManager::Instance().GetMeshPreset(MeshPreset::Cylinder), renderer);
	XMFLOAT3 offset4 = { -1.0f, 0.0f, 8.0f };
	tileEntity4->SetPosition(GetPosition().x + offset4.x, GetPosition().y + offset4.y, GetPosition().z + offset4.z);
	tileEntity4->SetScale({ 0.1f, 1.5f, 0.1f });
	tileEntity4->RotateX(XM_PIDIV2);
	tileEntity4->RotateY(XMConvertToRadians(315));
	GetEntities().push_back(tileEntity4);
	GetLocalOffset().push_back(offset4);

	Entity* tileEntity3 = ecs->CreateEntity<Entity>();
	tileEntity3->AddComponent<MeshRendererComponent>()->SetMesh(ResourceManager::Instance().GetMeshPreset(MeshPreset::Cylinder), renderer);
	XMFLOAT3 offset3 = { -2.0f, 0.0f, 12.0f };
	tileEntity3->SetPosition(GetPosition().x + offset3.x, GetPosition().y + offset3.y, GetPosition().z + offset3.z);
	tileEntity3->SetScale({ 0.1f, 3.0f, 0.1f });
	tileEntity3->RotateX(XM_PIDIV2);
	GetEntities().push_back(tileEntity3);
	GetLocalOffset().push_back(offset3);

	SetActive(false);
	SetIsTurnL(false);
	SetIsTurnR(false);
	SetIn(0);
	SetOut(-2);
}

void Tile1::Update(float deltaTime)
{
	
	MoveZ(-(GetSpeed() * deltaTime));

	for (int i = 0; i < GetEntities().size(); i++)
	{
		Entity* entity = GetEntities()[i];
		XMFLOAT3 offset = GetLocalOffset()[i];

		entity->SetActive(true);
		entity->SetPosition(GetPosition().x + offset.x,GetPosition().y + offset.y,GetPosition().z + offset.z);
	}

}