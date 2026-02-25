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
#include "Engine/ECS/Components/RigidBodyComponent.h"
#include "Engine/Projectile.h"
#include "Engine/ECS/Components/ColliderComponent.h"

#include <iostream>

using namespace core;

void GameScene::Initialize(ECS* ecs, Renderer* renderer, Engine* engine, Entity* camera)
{
	m_ecs = ecs;
	m_renderer = renderer;
	m_engine = engine;
	m_cam = camera;

	// -[TEST OBJECTS]- //
	m_cylinder = CreateCylinder();
	m_cube = CreateCube();
	m_sphere = CreateSphere();
	m_moon = CreateMoon();
	m_floor = CreateFloor();

	m_projectile = new Projectile();
	m_projectile->Initialize(0.1f, Shape::SPHERE, 15.0f, 3.0f, m_engine, m_ecs, m_renderer);
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
	m_deltaTime = dt;

	m_projectile->Update(deltaTime);
	//Test on entities
	m_cube->RotateX(XMConvertToRadians(.01f));
	m_cube->RotateLocalY(XMConvertToRadians(.05f));
	m_cube->RotateZ(XMConvertToRadians(.05f));

	m_moon->OrbitAround(m_cylinder->GetPosition(), m_cylinder->m_up, XMConvertToRadians(45.f * m_deltaTime), 1);
	m_moon->RotateLocalY(XMConvertToRadians(60.f * m_deltaTime));
	m_cylinder->RotateY(XMConvertToRadians(45.f * m_deltaTime));

	ProceduralRails();
	MoveCamera();
	
	Debug();

	if(Input::GetKeyDown(Keyboard::UP_ARROW))
	{
		m_sphere->MoveForward(0.5f);
	}
	if(Input::GetMouseButtonDown(Mouse::LEFT))
	{
		Entity* temp = m_projectile->GetAvaibleProjectile();
		if (temp == nullptr) return; // No projectile available in the pool

		temp->SetPosition(0.0f,0.0f,0.0f);
		temp->GetComponent<RigidBodyComponent>()->SetVelocity({ 0.0f, 0.0f, 5.0f });
	}
}

// -[TEST ENTITIES]- //
Entity* GameScene::CreateSphere()
{
	Entity* sphere = m_ecs->CreateEntity<Entity>();
	sphere->AddComponent<MeshRendererComponent>()->SetMesh(RessourceManager::Instance().GetSphere(), m_renderer);
	sphere->SetPosition(0.0f, 0.0f, 0.0f);
	sphere->AddComponent<PlayerComponent>();
	sphere->AddComponent<ColliderComponent>()->SetType(ColliderComponent::Type::Sphere);

	m_entities.push_back(sphere);
	return sphere;
}

Entity* GameScene::CreateCylinder()
{
	Entity* cylinder = m_ecs->CreateEntity<Entity>();
	cylinder->AddComponent<MeshRendererComponent>()->SetMesh(RessourceManager::Instance().GetCylinder(), m_renderer);
	cylinder->SetPosition(2.0f, 0.0f, 5.0f);
	cylinder->SetScale(.3f);
	m_entities.push_back(cylinder);
	return cylinder;
}

Entity* GameScene::CreateCube() {
	Entity* cube = m_ecs->CreateEntity<Entity>();
	cube->AddComponent<MeshRendererComponent>()->SetMesh(RessourceManager::Instance().GetCube(), m_renderer);
	cube->SetPosition(2.0f, 0.0f, 10.0f);
	cube->SetScale(.5f);
	//cube->AddComponent<StateMachineComponent>();
	/*StateMachineComponent* smc = cube->GetComponent<StateMachineComponent>();
	smc->SetStateMachine(cube, m_ecs);
	smc->GetStateMachine()->SetOwner(cube);
	smc->GetStateMachine()->ChangeState(new EnemyIdleState());*/

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

Entity* GameScene::CreateFloor()
{
	Entity* floor = m_ecs->CreateEntity<Entity>();
	floor->AddComponent<MeshRendererComponent>()->SetMesh(RessourceManager::Instance().GetCylinder(), m_renderer);
	floor->SetPosition(0,-2.5f,0);
	floor->SetScaleVector({ 50,0.1f,50 });
	m_entities.push_back(floor);
	return floor;
}



// -[RAIL GENERATION]- //
void GameScene::CreateRail()
{
	m_rail = m_ecs->CreateEntity<Entity>();
	m_rail->AddComponent<MeshRendererComponent>()->SetMesh(RessourceManager::Instance().GetCylinder(), m_renderer);
	m_rail->SetScaleVector({ .1f, 3.0f, .1f });
	m_rail->RotateX(XM_PIDIV2);
	m_entities.push_back(m_rail);
}

void GameScene::ProceduralRails()
{
	if (Input::GetKeyDown(Keyboard::SPACE)) {
		CreateRail();
		float nextPos = 0 ;
		if (m_rails.size() > 0) nextPos = m_rail->GetScale().y + m_rails.back()->GetPosition().z;

		m_rail->SetPosition(0.0f, -1.0f, nextPos);
		m_rails.push_back(m_rail);

		if (m_rails.size() > m_maxRails) {
			DeleteRails();
		}
	}
}

void GameScene::DeleteRails() {
	m_rails.front()->~Entity();
	m_rails.erase(m_rails.begin(), m_rails.begin() + 1);
}

// -[CAMERA]- //
void GameScene::MoveCamera()
{
	std::vector<float> delta = Input::GetMouseDelta();

	// Kinda Cheating, can cause problem when player is being rotated;

	m_cam->RotateY(XMConvertToRadians(delta[0]));
	m_cam->RotateLocalX(XMConvertToRadians(delta[1]));

	// WASD / ZQSD movement
	float directionForward = 0;
	float directionRight = 0;

	if (Input::GetKey(Keyboard::Z) || Input::GetKey(Keyboard::W))
	{
		directionForward += 1;
	}	
	if (Input::GetKey(Keyboard::S))
	{
		directionForward -= 1;
	};

	if (Input::GetKey(Keyboard::A) || Input::GetKey(Keyboard::Q))
	{
		directionRight -= 1;
	}
	if (Input::GetKey(Keyboard::D))
	{
		directionRight += 1;
	}

	m_cam->MoveRight((directionRight * m_speedPlayer * m_deltaTime));
	m_cam->MoveForward((directionForward * m_speedPlayer * m_deltaTime));
}

// -[DEBUG]- //
void GameScene::Debug() {
	if (Input::GetMouseButton(Mouse::LEFT)) {

	}
}


