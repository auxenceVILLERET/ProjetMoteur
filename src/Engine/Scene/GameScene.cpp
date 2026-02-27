#include "GameScene.h"
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
#include "ProjetMoteur/Tiles/TilesManager.h"

#include "Window.h"

using namespace core;

void GameScene::Initialize(ECS* ecs, Renderer* renderer, Engine* engine, Entity* camera)
{
	m_ecs = ecs;
	m_renderer = renderer;
	m_engine = engine;
	m_cam = camera;

	m_body = CreateBody();
	m_floor = CreateFloor();

	m_projectile = new Projectile();
	m_projectile->Initialize(0.1f, Shape::SPHERE, 15.0f, 3.0f, m_engine, m_ecs, m_renderer);

	m_tilesManager = new TilesManager();
	m_tilesManager->Initialize(m_ecs, m_renderer);
	// Ugly but needed until fix
	for (Entity* entity : m_entities)
	{
		entity->SetActive(false);
	}

}

void GameScene::OnEnter()
{
	m_tilesManager->OnEnter();
	for (Entity* entity : m_entities)
	{
		entity->SetActive(true);
	}
	UIFrame frame;
	frame.showSplash = false;
	frame.score = 0;
	m_renderer->SetUiFrame(frame);
}

void GameScene::OnExit()
{
	// Clean up entities 
	m_tilesManager->OnExit();
	for (Entity* entity : m_entities)
	{
		entity->SetActive(false);
	}
}

void GameScene::Update(float dt)
{
	m_deltaTime = dt;

	m_projectile->Update(m_deltaTime);
	m_tilesManager->Update(dt);
	
	// Make a few rails by default
	if (m_rails.size() < 3) ProceduralRails();	
	else m_startupFlag = false;

	ProceduralRails();
	MovePlayer();
	MoveCamera();
	Shooting();

	Debug();

	Shooting();
	
}

Entity* GameScene::CreateBody()
{
	Entity* body = m_ecs->CreateEntity<Entity>();
	body->AddComponent<MeshRendererComponent>()->SetMesh(ResourceManager::Instance().GetMeshPreset(MeshPreset::Cylinder), m_renderer);
	body->SetPosition(0, 0, 5);
	body->SetScale({ .25,.3,.25 });
	m_entities.push_back(body);
	return body;
}

Entity* GameScene::CreateFloor()
{
	Entity* floor = m_ecs->CreateEntity<Entity>();
	floor->AddComponent<MeshRendererComponent>()->SetMesh(ResourceManager::Instance().GetMeshPreset(MeshPreset::Cylinder), m_renderer);
	floor->SetPosition(0, -2.5f, 0);
	floor->SetScale({ 50,0.1f,50 });
	m_entities.push_back(floor);
	return floor;
}

// -[RAIL GENERATION]- //
void GameScene::CreateRail()
{
	m_rail = m_ecs->CreateEntity<Entity>();
	m_rail->AddComponent<MeshRendererComponent>()->SetMesh(ResourceManager::Instance().GetMeshPreset(MeshPreset::Cylinder), m_renderer);
	m_rail->SetScale({ .1f, 3.0f, .1f });
	m_rail->RotateX(XM_PIDIV2);
	m_entities.push_back(m_rail);
}

void GameScene::ProceduralRails() // TEMP
{
	if (Input::GetKeyDown(Keyboard::SPACE) || m_startupFlag) {
		CreateRail();
		float nextPos = 0;

		if (m_rails.size() > 0)
		{
			nextPos = m_rail->GetScale().y + m_rails.back()->GetPosition().z;
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
	std::cout << delta[0] << " " << delta[1] << std::endl;
	float sensitivity = 0.0025f;

	m_yaw += delta[0] * sensitivity;
	m_pitch += delta[1] * sensitivity;

	m_pitch = std::clamp(m_pitch, -1.4f, 1.4f);
	UpdateCameraTransform();
}

void GameScene::UpdateCameraTransform()
{
	// Position = body
	m_cam->SetPosition(m_body->GetPosition());
	//m_cam->Translate(0, 0, -3);

	// Load body rotation 
	XMVECTOR bodyQ = XMLoadFloat4(&m_body->m_quaternion);

	// Up local body
	XMVECTOR bodyUp = XMVector3Rotate(
		XMVectorSet(0, 1, 0, 0),
		bodyQ
	);

	// YAW around up local
	XMVECTOR yawQ = XMQuaternionRotationAxis(bodyUp, m_yaw);

	// Rotation apres yaw
	XMVECTOR yawedBody = XMQuaternionMultiply(bodyQ,yawQ);

	// Right local apres yaw
	XMVECTOR bodyRight = XMVector3Rotate(
		XMVectorSet(1, 0, 0, 0),
		yawedBody
	);

	// Pitch around right
	XMVECTOR pitchQ = XMQuaternionRotationAxis(bodyRight, m_pitch);

	// Rotation
	XMVECTOR finalQ = XMQuaternionMultiply( yawedBody, pitchQ );
	finalQ = XMQuaternionNormalize(finalQ);

	XMFLOAT4 qFloat;
	XMStoreFloat4(&qFloat, finalQ);

	m_cam->SetRotation(qFloat);
}

void GameScene::MovePlayer() {
	float offset = 2;

	// ROTATION AUTOUR DU RAIL
	if (Input::GetKey(Keyboard::LEFT))
	{
		m_body->OrbitAround({ 0,-1,5 },{0,0,1}, XMConvertToRadians(90.0f * m_deltaTime), offset);
		m_body->RotateLocalZ(XMConvertToRadians(90.0f * m_deltaTime));

	}
	if (Input::GetKey(Keyboard::RIGHT))
	{
		m_body->OrbitAround({ 0,-1,5 }, { 0,0,1 }, XMConvertToRadians(-90.0f * m_deltaTime), offset);
		m_body->RotateLocalZ(XMConvertToRadians(-90.0f * m_deltaTime));
	}
}

// -[SHOOTING]-
void GameScene::Shooting() {
	if (Input::GetMouseButtonDown(Mouse::LEFT))
	{
		Entity* temp = m_projectile->GetAvaibleProjectile();
		if (temp == nullptr) return; // No projectile available in the pool

		// hadle speed and direction of bullet
		XMFLOAT3 bulletSpeed = m_cam->GetForward();
		float speedMult = 5;
		bulletSpeed.x *= speedMult; bulletSpeed.y *= speedMult; bulletSpeed.z *= speedMult;

		// shoot from player
		temp->SetPosition(m_cam->GetPosition());
		temp->GetComponent<RigidBodyComponent>()->SetVelocity(bulletSpeed);
			
	}
}

// -[DEBUG]- //
void GameScene::Debug() {
	Window* window = Window::GetInstance();
	
	if (window->IsCursorLocked())
	{
		SetCursorPos(
			window->GetCursorCenter().x,
			window->GetCursorCenter().y
		);
	}
}


