#include "SandboxScene.h"
#include "Engine/ECS/Entity.h"
#include "Engine/ResourceManager.h" 
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

void SandboxScene::Initialize(ECS* ecs, Renderer* renderer, Engine* engine, Entity* camera)
{
	m_ecs = ecs;
	m_renderer = renderer;
	m_engine = engine;
	m_cam = camera;

	m_cam->SetPosition(0.0f, 0.0f, 0.0f);
	// -[TEST OBJECTS]- //
	m_cylinder = CreateCylinder();
	m_cylinder->RotateX(XMConvertToRadians(90.f));
	m_cube = CreateCube();
	m_sphere = CreateSphere();
	m_moon = CreateMoon();
	m_moonMoon = CreateMoonMoon();
	m_floor = CreateFloor();

	m_projectile = new Projectile();
	m_projectile->Initialize(0.1f, Shape::SPHERE, 15, 3.0f, m_engine, m_ecs, m_renderer);

	// Ugly but needed until fix
	for (Entity* entity : m_entities)
	{
		entity->SetActive(false);
	}
}

void SandboxScene::OnEnter()
{
	for (Entity* entity : m_entities)
	{
		entity->SetActive(true);
	}
}

void SandboxScene::OnExit()
{
	// Clean up entities 
	for (Entity* entity : m_entities)
	{
		entity->SetActive(false);
	}
}

void SandboxScene::Update(float dt)
{
	m_deltaTime = dt;

	m_projectile->Update(m_deltaTime);

	//Test on entities
	//m_cube->RotateX(XMConvertToRadians(.01f));
	//m_cube->RotateLocalY(XMConvertToRadians(.5f));
	//m_cube->RotateZ(XMConvertToRadians(5.f));

	m_angleOrbit += XMConvertToRadians(90.0F * m_deltaTime);

	m_moon->OrbitAround(m_cylinder->GetPosition(), { 0.0f,1.0f,0.0f }, m_angleOrbit, 1.0f);
	m_moonMoon->OrbitAround(m_moon->GetPosition(), {0.0f,1.0f,0.0f}, 0.0f, .25f);


	MoveCamera();
	Debug();
	Shooting();
}

// -[TEST ENTITIES]- //
Entity* SandboxScene::CreateSphere()
{
	Entity* sphere = m_ecs->CreateEntity<Entity>();
	sphere->AddComponent<MeshRendererComponent>()->SetMesh(ResourceManager::Instance().GetMeshPreset(MeshPreset::Sphere), m_renderer);
	sphere->SetPosition(-1.0f, 0.0f, 10.0f);
	sphere->AddComponent<PlayerComponent>();
	sphere->AddComponent<ColliderComponent>()->SetType(ColliderComponent::Type::Sphere);

	m_entities.push_back(sphere);
	return sphere;
}

Entity* SandboxScene::CreateCylinder()
{
	Entity* cylinder = m_ecs->CreateEntity<Entity>();
	cylinder->AddComponent<MeshRendererComponent>()->SetMesh(ResourceManager::Instance().GetMeshPreset(MeshPreset::Cylinder), m_renderer);
	cylinder->SetPosition(0.0f, -2.0f, 2.0f);
	cylinder->SetScale({1.0f, 5.0f, 1.0f});
	m_entities.push_back(cylinder);
	return cylinder;
}

Entity* SandboxScene::CreateCube() 
{
	Entity* cube = m_ecs->CreateEntity<Entity>();
	cube->AddComponent<MeshRendererComponent>()->SetMesh(ResourceManager::Instance().GetMeshPreset(MeshPreset::Cube), m_renderer);
	cube->SetPosition(0.0f, 0.0f, 0.0f);
	cube->SetScale(1.0f);

	m_entities.push_back(cube);
	return cube;
}

Entity* SandboxScene::CreateMoon() 
{
	Entity* moon = m_ecs->CreateEntity<Entity>();
	moon->AddComponent<MeshRendererComponent>()->SetMesh(ResourceManager::Instance().GetMeshPreset(MeshPreset::Sphere), m_renderer);
	moon->SetPosition(m_cylinder->GetPosition().x, m_cylinder->GetPosition().y, m_cylinder->GetPosition().z);
	moon->SetScale(.1f);
	m_entities.push_back(moon);
	return moon;
}

Entity* SandboxScene::CreateMoonMoon() {
	Entity* moonMoon = m_ecs->CreateEntity<Entity>();
	moonMoon->AddComponent<MeshRendererComponent>()->SetMesh(ResourceManager::Instance().GetMeshPreset(MeshPreset::Sphere), m_renderer);
	moonMoon->SetPosition(m_moon->GetPosition().x, m_moon->GetPosition().y, m_moon->GetPosition().z);
	moonMoon->SetScale(.05f);
	m_entities.push_back(moonMoon);
	return moonMoon;
}

Entity* SandboxScene::CreateFloor()
{
	Entity* floor = m_ecs->CreateEntity<Entity>();
	floor->AddComponent<MeshRendererComponent>()->SetMesh(ResourceManager::Instance().GetMeshPreset(MeshPreset::Cylinder), m_renderer);
	floor->SetPosition(0, -2.5f, 0);
	floor->SetScale({ 50,0.1f,50 });
	m_entities.push_back(floor);
	return floor;
}

// -[RAIL GENERATION]- //
void SandboxScene::CreateRail()
{
	m_rail = m_ecs->CreateEntity<Entity>();
	m_rail->AddComponent<MeshRendererComponent>()->SetMesh(ResourceManager::Instance().GetMeshPreset(MeshPreset::Cylinder), m_renderer);
	m_rail->SetScale({ .1f, 3.0f, .1f });
	m_rail->RotateX(XM_PIDIV2);
	m_entities.push_back(m_rail);
}

void SandboxScene::ProceduralRails()
{
	if (Input::GetKeyDown(Keyboard::SPACE)) {
		CreateRail();
		float nextPos = 0;

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

void SandboxScene::DeleteRails() {
	m_rails.front()->~Entity();
	m_rails.erase(m_rails.begin(), m_rails.begin() + 1);
}

// -[CAMERA]- //
void SandboxScene::MoveCamera()
{
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

// -[SHOOTING]-
void SandboxScene::Shooting() {
	if (Input::GetMouseButtonDown(Mouse::LEFT))
	{
		Entity* temp = m_projectile->GetAvaibleProjectile();
		if (temp == nullptr) return; // No projectile available in the pool

		temp->SetPosition(0.0f, 0.0f, 0.0f);
		temp->GetComponent<RigidBodyComponent>()->SetVelocity({ 0.0f, 0.0f, 5.0f });
	}
}

// -[DEBUG]- //
void SandboxScene::Debug() {

}


