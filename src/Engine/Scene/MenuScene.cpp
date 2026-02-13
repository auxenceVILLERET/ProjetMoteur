#include "MenuScene.h"
#include "Engine/ECS/Entity.h"
#include "Engine/RessourceManager.h" 
#include "Engine/ECS/Components/MeshRendererComponent.h"
#include "Render/Renderer.h"
#include "Engine/ECS/ECS.h"

void MenuScene::Initialize(ECS* ecs, Renderer* renderer, Engine* engine)
{
	m_engine = engine;
	m_ecs = ecs;
	m_renderer = renderer;

	Entity* menuEntity = m_ecs->CreateEntity<Entity>();
	menuEntity->AddComponent<MeshRendererComponent>()->SetMesh(RessourceManager::Instance().GetCube(), m_renderer);
	
	menuEntity->SetPosition(0.0f, 0.0f, 5.0f);
	
	m_entities.push_back(menuEntity);
}

void MenuScene::OnEnter()
{
	for(Entity* entity : m_entities) 
	{
		entity->SetActive(true);
	}
}

void MenuScene::OnExit()
{
	// Clean up entities 
	for (Entity* entity : m_entities) 
	{
		entity->SetActive(false);
	}
}

void MenuScene::Update(float dt)
{
}
