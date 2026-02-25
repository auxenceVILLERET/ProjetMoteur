#include "App.h"
#include <Window.h>
#include <Renderer.h>
#include <iostream>
#include "Engine/Engine.h"
#include "Core/InputsMethods.h"
#include "Engine/Scene/SceneManager.h"
#include "Engine/ECS/Components/CameraComponent.h"
#include "Engine/ECS/Systems/CameraSystem.h"
#include "Engine/RessourceManager.h"
#include "Engine/ECS/Components/MeshRendererComponent.h"
#include "Render/UploadContext.h" 
#include "Mesh.h"
#include "Engine/ECS/Systems/RenderSystem.h"
#include "Engine/Scene/GameScene.h"
#include "Engine/Scene/MenuScene.h"
#include "Engine/ECS/Systems/StateMachineSystem.h"
#include "Engine/ECS/Systems/PhysicSystem.h"
#include "Engine/ECS/Systems/ColliderSystem.h"

using namespace core;

App::App(Engine& engine) : m_engine(engine)
{
	engine.SetInitCallback(std::bind(&App::Initialize, this));
	engine.SetUpdateCallback(std::bind(&App::Update, this));
	engine.SetShutdownCallback(std::bind(&App::Shutdown, this));

	m_renderer = nullptr;
	m_window = nullptr;
	m_sceneManager = nullptr;
	m_cam = nullptr;
	m_ecs = nullptr;
}

void App::Initialize()
{	
	// Initialize your application here
	m_window = new Window(800, 600, L"test");
	m_ecs = &m_engine.GetECS();
	CreateCamera();
	m_renderer = new Renderer();
	m_sceneManager = new SceneManager();

	m_renderer->Initialize(m_window, m_cam);

	UploadContext* uploader = m_renderer->GetUploadContext();

	uploader->Begin();

	RessourceManager::Instance().Initialize(m_renderer, uploader);

	RessourceManager::Instance().GetCube();
	RessourceManager::Instance().GetCylinder();
	RessourceManager::Instance().GetSphere();

	uploader->EndAndWait();
	RessourceManager::Instance().FinalizeUpload();

	m_ecs->AddSystem<RenderSystem>()->SetRenderer(m_renderer);
	
	m_sceneManager->CreateScene<GameScene>("Game")->Initialize(m_ecs, m_renderer, &m_engine);
	m_sceneManager->CreateScene<MenuScene>("Menu")->Initialize(m_ecs, m_renderer, &m_engine);
	
	m_sceneManager->ChangeScene("Menu");

	m_ecs->AddSystem<StateMachineSystem>();
	m_ecs->AddSystem<PhysicSystem>();
	m_ecs->AddSystem<ColliderSystem>();
}

void App::Update()
{
	UpdateWindow();
	m_sceneManager->Update(m_engine.GetDeltaTime());

	// Update your application here
	HandleInput();
	//cam movement
	Movement();
}

void App::Shutdown()
{
	// Clean up your application here
}

void App::UpdateWindow()
{
	m_window->ProcessMessages();
	m_renderer->Update();

	if(m_window->IsOpen() == false)
		m_engine.Shutdown();
}

void App::HandleInput()
{
	if(Input::GetKey(Keyboard::A))
	{
		m_sceneManager->ChangeScene("Game");
	}
	if (Input::GetKey(Keyboard::E))
	{
		m_sceneManager->ChangeScene("Menu");
	}
	
	Input::Update();
}

void App::CreateCamera()
{
	m_cam = m_ecs->CreateEntity<Entity>();
	m_cam->AddComponent<CameraComponent>();
	m_ecs->AddSystem<CameraSystem>();

	m_cam->SetPosition(0.0f, 0.0f, 0.0f);

	m_cam->GetComponent<CameraComponent>()->SetFOV(45.0f);

	m_cam->GetComponent<CameraComponent>()->SetAll(
		0.1f,
		100.0f,
		static_cast<float>(m_window->GetHeight()),
		static_cast<float>(m_window->GetWidth()),
		true
	);
	XMFLOAT4X4 identityMatrix;
	XMStoreFloat4x4(&identityMatrix, XMMatrixIdentity());
	m_cam->GetComponent<CameraComponent>()->SetViewMatrix(identityMatrix);
}

// MOVEMENT OF THE CAMERA
void App::Movement()
{
	if (Input::GetKey(Keyboard::Z)) {
		m_cam->MoveForward((1.0f * m_speedPlayer)); // WorldMatrix is not updated
		m_cam->CoutMatrix();

	}
}
