#include "GameScene.h"
#include "Engine/ECS/Entity.h"
#include "Engine/RessourceManager.h" 
#include "Engine/ECS/Components/MeshRendererComponent.h"
#include "Render/Renderer.h"
#include "Engine/ECS/ECS.h"
#include "Engine/ECS/Components/PlayerComponent.h"
#include "Engine/Engine.h"
#include "Core/InputsMethods.h"
#include "Engine/ECS/Components/StateMachineComponent.h"
#include "ProjetMoteur/EnemyIdleState.h"
#include "ProjetMoteur/EnemyChaseState.h"

using namespace core;

void GameScene::Initialize(ECS* ecs, Renderer* renderer, Engine* engine)
{
	m_ecs = ecs;
	m_renderer = renderer;
	m_engine = engine;

	//TEST OBJECTS
	m_cylinder = CreateCylinder();
	m_cube = CreateCube();
	m_sphere = CreateSphere();
	m_moon = CreateMoon();
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
	//Test on entities
	m_cube->SetRotationX(XMConvertToRadians(.01f));
	m_cube->SetRotationY(XMConvertToRadians(.05f));
	m_cube->SetRotationZ(XMConvertToRadians(.05f));


	m_moon->SetPositionY(m_cylinder->GetPosition().y + .75f);

	ProceduralRails();

	if(Input::GetKeyDown(Keyboard::UP_ARROW))
	{
		m_sphere->MoveForward(.5f);
	}

}

// TEST ENTITIES
Entity* GameScene::CreateSphere()
{
	Entity* sphere = m_ecs->CreateEntity<Entity>();
	sphere->AddComponent<MeshRendererComponent>()->SetMesh(RessourceManager::Instance().GetSphere(), m_renderer);
	sphere->SetPosition(0.0f, 0.0f, 0.0f);
	sphere->AddComponent<PlayerComponent>();

	m_entities.push_back(sphere);
	return sphere;
}

Entity* GameScene::CreateCylinder()
{
	Entity* cylinder = m_ecs->CreateEntity<Entity>();
	cylinder->AddComponent<MeshRendererComponent>()->SetMesh(RessourceManager::Instance().GetCylinder(), m_renderer);
	cylinder->SetPosition(2.0f, 0.0f, 5.0f);
	cylinder->SetRotationX(XMConvertToRadians(45.0f));
	cylinder->SetScale(.3f);
	m_entities.push_back(cylinder);
	return cylinder;
}

Entity* GameScene::CreateCube() {
	Entity* cube = m_ecs->CreateEntity<Entity>();
	cube->AddComponent<MeshRendererComponent>()->SetMesh(RessourceManager::Instance().GetCube(), m_renderer);
	cube->SetPosition(0.0f, 0.0f, 10.0f);
	cube->SetScale(.5f);
	cube->AddComponent<StateMachineComponent>();
	cube->GetComponent<StateMachineComponent>()->SetStateMachine(cube, m_ecs);
	StateMachineComponent* smc = cube->GetComponent<StateMachineComponent>();
	smc->GetStateMachine()->SetOwner(cube);
	smc->GetStateMachine()->ChangeState(new EnemyIdleState());

	m_entities.push_back(cube);
	return cube;
}

Entity* GameScene::CreateMoon() {
	Entity* moon = m_ecs->CreateEntity<Entity>();
	moon->AddComponent<MeshRendererComponent>()->SetMesh(RessourceManager::Instance().GetSphere(), m_renderer);
	moon->SetPosition(m_cylinder->GetPosition().x, m_cylinder->GetPosition().y, m_cylinder->GetPosition().z);
	moon->SetScale(.1f);
	m_entities.push_back(moon);
	return moon;
}
///////////////////

// RAIL GENERATION
void GameScene::CreateRail()
{
	m_rail = m_ecs->CreateEntity<Entity>();
	m_rail->AddComponent<MeshRendererComponent>()->SetMesh(RessourceManager::Instance().GetCylinder(), m_renderer);
	m_rail->SetScaleVector({ .1f, 3.0f, .1f });
	m_rail->SetRotationX(XM_PIDIV2);
	m_entities.push_back(m_rail);
}

void GameScene::ProceduralRails()
{
	if (Input::GetKeyDown(Keyboard::SPACE)) {
		CreateRail();
		m_rail->SetPosition(0.0f, -1.0f, m_rail->GetScale().y * m_rails.size());
		m_rails.push_back(m_rail);
	}
}
///////////////////
