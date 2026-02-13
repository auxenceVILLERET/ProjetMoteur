#include "GameScene.h"
#include "Engine/ECS/Entity.h"
#include "Engine/RessourceManager.h" 
#include "Engine/ECS/Components/MeshRendererComponent.h"
#include "Render/Renderer.h"
#include "Engine/ECS/ECS.h"


void GameScene::Initialize(ECS* ecs, Renderer* renderer)
{
	m_ecs = ecs;
	m_renderer = renderer;

	Entity* player = m_ecs->CreateEntity<Entity>();
	player->AddComponent<MeshRendererComponent>()->SetMesh(RessourceManager::Instance().GetSphere(), m_renderer);

	player->SetPosition(-2.0f, 0.0f, 5.0f);
	m_entities.push_back(player);
}

void GameScene::OnEnter()
{
	for(Entity* entity : m_entities) 
	{
		entity->SetActive(true);
	}
}

void GameScene::OnExit()
{
	// Clean up entities 
	for (Entity* entity : m_entities) 
	{
		entity->SetActive(false);
	}
}

void GameScene::Update(float dt)
{

}
