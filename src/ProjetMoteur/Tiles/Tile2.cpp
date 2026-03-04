#include "Tile2.h"
#include "Engine/ECS/ECS.h"
#include "Render/Renderer.h"
#include "Engine/ECS/Entity.h"
#include "Engine/ResourceManager.h"
#include "Engine/ECS/Components/MeshRendererComponent.h"
#include "Engine/ECS/Components/EnemyComponent.h"
#include "Engine/ECS/Components/ColliderComponent.h"
#include "Engine/ECS/Components/ObstacleComponent.h"

void Tile2::Initialize(ECS* ecs, Renderer* renderer)
{
	TextureHandle texture = ResourceManager::Instance().LoadTexture(L"../../res/SplashScreen.png");
	TextureHandle wallTexture = ResourceManager::Instance().LoadTexture(L"../../res/smiley.png");

	Entity* tileEntity = ecs->CreateEntity<Entity>();
	tileEntity->AddComponent<MeshRendererComponent>()->SetMesh(ResourceManager::Instance().GetMeshPreset(MeshPreset::Cylinder), renderer);
	XMFLOAT3 offset = { -2.0f, 0.0f, 0.0f };
	tileEntity->SetPosition(GetPosition().x + offset.x, GetPosition().y + offset.y, GetPosition().z + offset.z);
	tileEntity->SetScale({ 0.1f, 4.0f, 0.1f });
	tileEntity->RotateX(XM_PIDIV2);
	tileEntity->GetComponent<MeshRendererComponent>()->SetTexture(texture);
	GetEntities().push_back(tileEntity);
	GetLocalOffset().push_back(offset);

	Entity* tileEntity1 = ecs->CreateEntity<Entity>();
	tileEntity1->AddComponent<MeshRendererComponent>()->SetMesh(ResourceManager::Instance().GetMeshPreset(MeshPreset::Cylinder), renderer);
	XMFLOAT3 offset1 = { -2.0f, 0.0f, 4.0f };
	tileEntity1->SetPosition(GetPosition().x + offset1.x, GetPosition().y + offset1.y, GetPosition().z + offset1.z);
	tileEntity1->SetScale({ 0.1f, 2.8f, 0.1f });
	tileEntity1->RotateX(XM_PIDIV2);
	tileEntity1->RotateY(XMConvertToRadians(45));
	tileEntity1->GetComponent<MeshRendererComponent>()->SetTexture(texture);
	GetEntities().push_back(tileEntity1);
	GetLocalOffset().push_back(offset1);

	Entity* tileEntity2 = ecs->CreateEntity<Entity>();
	tileEntity2->AddComponent<MeshRendererComponent>()->SetMesh(ResourceManager::Instance().GetMeshPreset(MeshPreset::Cylinder), renderer);
	XMFLOAT3 offset2 = { 0.0f, 0.0f, 6.0f };
	tileEntity2->SetPosition(GetPosition().x + offset2.x, GetPosition().y + offset2.y, GetPosition().z + offset2.z);
	tileEntity2->SetScale({ 0.1f, 2.0f, 0.1f });
	tileEntity2->RotateX(XM_PIDIV2);
	tileEntity2->GetComponent<MeshRendererComponent>()->SetTexture(texture);
	GetEntities().push_back(tileEntity2);
	GetLocalOffset().push_back(offset2);

	Entity* tileEntity4 = ecs->CreateEntity<Entity>();
	tileEntity4->AddComponent<MeshRendererComponent>()->SetMesh(ResourceManager::Instance().GetMeshPreset(MeshPreset::Cylinder), renderer);
	XMFLOAT3 offset4 = { 0.0f, 0.0f, 8.0f };
	tileEntity4->SetPosition(GetPosition().x + offset4.x, GetPosition().y + offset4.y, GetPosition().z + offset4.z);
	tileEntity4->SetScale({ 0.1f, 2.8f, 0.1f });
	tileEntity4->RotateX(XM_PIDIV2);
	tileEntity4->RotateY(XMConvertToRadians(45));
	tileEntity4->GetComponent<MeshRendererComponent>()->SetTexture(texture);
	GetEntities().push_back(tileEntity4);
	GetLocalOffset().push_back(offset4);

	Entity* tileEntity3 = ecs->CreateEntity<Entity>();
	tileEntity3->AddComponent<MeshRendererComponent>()->SetMesh(ResourceManager::Instance().GetMeshPreset(MeshPreset::Cylinder), renderer);
	XMFLOAT3 offset3 = { 2.0f, 0.0f, 10.0f };
	tileEntity3->SetPosition(GetPosition().x + offset3.x, GetPosition().y + offset3.y, GetPosition().z + offset3.z);
	tileEntity3->SetScale({ 0.1f, 6.0f, 0.1f });
	tileEntity3->RotateX(XM_PIDIV2);
	tileEntity3->GetComponent<MeshRendererComponent>()->SetTexture(texture);
	GetEntities().push_back(tileEntity3);
	GetLocalOffset().push_back(offset3);


	// Define m_rails vector //
	GetRails().push_back(tileEntity);
	GetRails().push_back(tileEntity1);
	GetRails().push_back(tileEntity2);
	GetRails().push_back(tileEntity4);
	GetRails().push_back(tileEntity3);

	Entity* EnemyEntity = ecs->CreateEntity<Entity>();
	EnemyEntity->AddComponent<MeshRendererComponent>()->SetMesh(ResourceManager::Instance().GetMeshPreset(MeshPreset::Cube), renderer);
	EnemyEntity->AddComponent<EnemyComponent>()->SetID(1);
	EnemyEntity->AddComponent<ColliderComponent>()->SetType(ColliderComponent::Type::Sphere);
	XMFLOAT3 offsetEnemy = { 2.0f, 1.0f, 6.0f };
	EnemyEntity->SetPosition(GetPosition().x + offsetEnemy.x, GetPosition().y + offsetEnemy.y, GetPosition().z + offsetEnemy.z);
	EnemyEntity->SetScale({ 0.5f, 0.5f, 0.5f });
	EnemyEntity->GetComponent<MeshRendererComponent>()->SetTexture(texture);
	EnemyEntity->SetActive(false);
	EnemyEntity->GetComponent<ColliderComponent>()->SetRadius(0.5f);
	GetEntities().push_back(EnemyEntity);
	GetLocalOffset().push_back(offsetEnemy);

	Entity* EnemyEntity2 = ecs->CreateEntity<Entity>();
	EnemyEntity2->AddComponent<MeshRendererComponent>()->SetMesh(ResourceManager::Instance().GetMeshPreset(MeshPreset::Cube), renderer);
	EnemyEntity2->AddComponent<EnemyComponent>()->SetID(2);
	EnemyEntity2->AddComponent<ColliderComponent>()->SetType(ColliderComponent::Type::Sphere);
	XMFLOAT3 offsetEnemy2 = { -2.0f, 1.0f, 6.0f };
	EnemyEntity2->SetPosition(GetPosition().x + offsetEnemy2.x, GetPosition().y + offsetEnemy2.y, GetPosition().z + offsetEnemy2.z);
	EnemyEntity2->SetScale({ 0.5f, 0.5f, 0.5f });
	EnemyEntity2->GetComponent<MeshRendererComponent>()->SetTexture(texture);
	EnemyEntity2->SetActive(false);
	EnemyEntity2->GetComponent<ColliderComponent>()->SetRadius(0.5f);
	GetEntities().push_back(EnemyEntity2);
	GetLocalOffset().push_back(offsetEnemy2);

	Entity* EnemyEntity3 = ecs->CreateEntity<Entity>();
	EnemyEntity3->AddComponent<MeshRendererComponent>()->SetMesh(ResourceManager::Instance().GetMeshPreset(MeshPreset::Cube), renderer);
	EnemyEntity3->AddComponent<EnemyComponent>()->SetID(3);
	EnemyEntity3->AddComponent<ColliderComponent>()->SetType(ColliderComponent::Type::Sphere);
	XMFLOAT3 offsetEnemy3 = { 0.0f, 1.0f, 14.0f };
	EnemyEntity3->SetPosition(GetPosition().x + offsetEnemy3.x, GetPosition().y + offsetEnemy3.y, GetPosition().z + offsetEnemy3.z);
	EnemyEntity3->SetScale({ 0.5f, 0.5f, 0.5f });
	EnemyEntity3->GetComponent<MeshRendererComponent>()->SetTexture(texture);
	EnemyEntity3->SetActive(false);
	EnemyEntity3->GetComponent<ColliderComponent>()->SetRadius(0.5f);
	GetEntities().push_back(EnemyEntity3);
	GetLocalOffset().push_back(offsetEnemy3);

	Entity* ObstacleEntity = ecs->CreateEntity<Entity>();
	ObstacleEntity->AddComponent<MeshRendererComponent>()->SetMesh(ResourceManager::Instance().GetMeshPreset(MeshPreset::Cube), renderer);
	ObstacleEntity->AddComponent<ObstacleComponent>();
	ObstacleEntity->AddComponent<ColliderComponent>()->SetType(ColliderComponent::Type::Sphere);
	XMFLOAT3 offsetObstacle = { 2.0f, 0.0f, 16.0f };
	ObstacleEntity->SetPosition(GetPosition().x + offsetObstacle.x, GetPosition().y + offsetObstacle.y, GetPosition().z + offsetObstacle.z);
	ObstacleEntity->SetScale({ 1.0f, 1.0f, 1.0f });
	ObstacleEntity->GetComponent<MeshRendererComponent>()->SetTexture(wallTexture);
	ObstacleEntity->SetActive(false);
	ObstacleEntity->GetComponent<ColliderComponent>()->SetRadius(1.0f);
	m_obstacle = ObstacleEntity;
	GetEntities().push_back(ObstacleEntity);
	GetLocalOffset().push_back(offsetObstacle);

	SetActive(false);
	SetIsTurnL(false);
	SetIsTurnR(false);
	SetIn(-2);
	SetOut(2);
}

void Tile2::Update(float deltaTime)
{
	MoveZ(-(GetSpeed() * deltaTime));

	for (int i = 0; i < GetEntities().size(); i++)
	{
		Entity* entity = GetEntities()[i];
		XMFLOAT3 offset = GetLocalOffset()[i];

		entity->SetPosition(GetPosition().x + offset.x, GetPosition().y + offset.y, GetPosition().z + offset.z);

		if (m_activeEnemy != nullptr)
		{
			if (m_activeEnemy->IsActive() == false)
			{
				int score = m_activeEnemy->GetComponent<EnemyComponent>()->GetScoreValue() + GetScoreValue();
				SetScoreValue(score);
				m_obstacle->SetActive(false);
				m_activeEnemy = nullptr;
			}
		}
	}
}