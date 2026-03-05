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
#include "ProjetMoteur/Tiles/Tile6.h"
#include "Window.h"
#include <iostream>

using namespace core;

void SandboxScene::Initialize(ECS* ecs, Renderer* renderer, Engine* engine, Entity* camera, SceneManager* sceneManager)
{
	m_ecs = ecs;
	m_renderer = renderer;
	m_engine = engine;
	m_cam = camera;
	m_sceneManager = sceneManager;

	m_cam->SetPosition(0.0f, 0.0f, 0.0f);

	Tile6* tile = new Tile6();
	tile->Initialize(m_ecs, m_renderer);
	tile->SetPosition({ 0.0f, 0.0f, 0.0f });
	tile->SetActive(false);

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

	MoveCamera();
	HandleCursor();
	Debug();
}

// -[LOCK & HIDE CURSOR + Calculate DeltaMouse]- //
void SandboxScene::HandleCursor()
{
	Window* window = Window::GetInstance();

	if (Input::GetKeyDown(Keyboard::ESC))
	{
		m_locked = !m_locked;
		window->LockCursor(m_locked);
	}

	if (!m_locked)
	{
		return;
	}

	window->UpdateCursorCenter();

	POINT mouse;
	GetCursorPos(&mouse);

	float dx = float(mouse.x - window->GetCursorCenter().x);
	float dy = float(mouse.y - window->GetCursorCenter().y);

	float sensitivity = 0.0025f;

	m_yaw += dx * sensitivity;
	m_pitch += dy * sensitivity;

	m_pitch = std::clamp(m_pitch, -1.0f, 1.0f);

	SetCursorPos(
		window->GetCursorCenter().x,
		window->GetCursorCenter().y
	);

	UpdateCameraTransform();
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


void SandboxScene::UpdateCameraTransform()
{
	// Quaternion Yaw (axe Y monde)
	XMVECTOR yawQ = XMQuaternionRotationAxis(
		XMVectorSet(0, 1, 0, 0),
		m_yaw
	);

	// Right vector après yaw
	XMVECTOR right = XMVector3Rotate(
		XMVectorSet(1, 0, 0, 0),
		yawQ
	);

	// Quaternion pitch
	XMVECTOR pitchQ = XMQuaternionRotationAxis(
		right,
		m_pitch
	);

	// Rotation finale
	XMVECTOR finalQ = XMQuaternionMultiply(yawQ, pitchQ);
	finalQ = XMQuaternionNormalize(finalQ);

	XMFLOAT4 q;
	XMStoreFloat4(&q, finalQ);

	m_cam->SetRotation(q);
}

// -[DEBUG]- //
void SandboxScene::Debug() {

}
