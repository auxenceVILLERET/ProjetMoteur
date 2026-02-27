#include "GameScene.h"
#include "Engine/ECS/Entity.h"
#include "Engine/ResourceManager.h" 
#include "Engine/ECS/Components/MeshRendererComponent.h"
#include "Render/Renderer.h"
#include "Engine/ECS/ECS.h"

void GameScene::Initialize(ECS* ecs, Renderer* renderer)
{
	m_ecs = ecs;
	m_renderer = renderer;

	Entity* player = m_ecs->CreateEntity<Entity>();
	MeshRendererComponent* mrc = player->AddComponent<MeshRendererComponent>();
	mrc->SetMesh(ResourceManager::Instance().GetMeshPreset(MeshPreset::Sphere), m_renderer);

	player->SetPosition(0.0f, 0.0f, 5.0f);
	m_entities.push_back(player);
}

void GameScene::OnEnter()
{
	for(Entity* entity : m_entities) 
	{
		entity->SetActive(true);
	}
	UIFrame frame;
	frame.showSplash = true;
	frame.score = 0;
	m_renderer->SetUiFrame(frame);
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
	m_entities[0]->SetRotationY(sinf(dt));
}
