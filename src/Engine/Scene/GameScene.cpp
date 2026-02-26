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

	m_body = m_ecs->CreateEntity<Entity>();

	// For Clarity
	m_floor = CreateFloor();

	m_projectile = new Projectile();
	m_projectile->Initialize(0.1f, Shape::SPHERE, 15.0f, 3.0f, m_engine, m_ecs, m_renderer);

	// Ugly but needed until fix
	for (Entity* entity : m_entities)
	{
		entity->SetActive(false);
	}

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

	m_projectile->Update(m_deltaTime);


	ProceduralRails();
	MovePlayer();
	MoveCamera();
	
	Debug();

	if(Input::GetMouseButtonDown(Mouse::LEFT))
	{
		Entity* temp = m_projectile->GetAvaibleProjectile();
		if (temp == nullptr) return; // No projectile available in the pool

		temp->SetPosition(0.0f,0.0f,0.0f);
		temp->GetComponent<RigidBodyComponent>()->SetVelocity({ 0.0f, 0.0f, 5.0f });
	}
}

Entity* GameScene::CreateFloor()
{
	Entity* floor = m_ecs->CreateEntity<Entity>();
	floor->AddComponent<MeshRendererComponent>()->SetMesh(RessourceManager::Instance().GetCylinder(), m_renderer);
	floor->SetPosition(0,-2.5f,0);
	floor->SetScale({ 50,0.1f,50 });
	m_entities.push_back(floor);
	return floor;
}

// -[RAIL GENERATION]- //
void GameScene::CreateRail()
{
	m_rail = m_ecs->CreateEntity<Entity>();
	m_rail->AddComponent<MeshRendererComponent>()->SetMesh(RessourceManager::Instance().GetCylinder(), m_renderer);
	m_rail->SetScale({ .1f, 3.0f, .1f });
	m_rail->RotateX(XM_PIDIV2);
	m_entities.push_back(m_rail);
}

void GameScene::ProceduralRails()
{
	if (Input::GetKeyDown(Keyboard::SPACE)) {
		CreateRail();
		float nextPos = 0 ;
		
			if (m_rails.size() > 0)
			{ 
				nextPos = m_rail->GetScale().y + m_rails.back()->GetPosition().z;
				m_cam->SetPosition({ m_rails.back()->GetPosition() });
				m_cam->Translate(0, 1, 0);
			}

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
	m_cam->SetPosition(m_body->GetPosition());
	
	m_body->RotateLocalY(XMConvertToRadians(delta[0]));
	m_cam->SetRotationY(m_body->GetRotation().y);

	m_cam->RotateLocalX(XMConvertToRadians(delta[1]));
}

void GameScene::MovePlayer() {
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

	m_body->MoveRight((directionRight * m_speedPlayer * m_deltaTime));
	m_body->MoveForward((directionForward * m_speedPlayer * m_deltaTime));
}

// -[DEBUG]- //
void GameScene::Debug() {
	if (Input::GetMouseButtonDown(Mouse::RIGHT)) {

		m_body->CoutRotation();
		m_cam->CoutRotation();
		std::cout << " ||||||||||||||||||||||||||||||||||||||| " << std::endl;
	}
}


