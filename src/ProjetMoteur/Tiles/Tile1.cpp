#include "Tile1.h"
#include "Engine/ECS/Entity.h"
#include "Engine/RessourceManager.h"
#include "Engine/ECS/Components/MeshRendererComponent.h"

void Tile1::Initialize(ECS* ecs, Renderer* renderer)
{
	Entity* tileEntity = ecs->CreateEntity<Entity>();
	tileEntity->AddComponent<MeshRendererComponent>()->SetMesh(RessourceManager::Instance().GetCylinder(), renderer);
	tileEntity->SetPosition(0.0f,0.0f,0.0f);
	tileEntity->SetScale(1.0f);
	GetEntities().push_back(tileEntity);

	SetActive(true);
	SetIsTurnL(false);
	SetIsTurnR(false);

}
