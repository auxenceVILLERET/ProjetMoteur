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

using namespace core;

App::App(Engine& engine) : m_engine(engine)
{
	engine.SetInitCallback(std::bind(&App::Initialize, this));
	engine.SetUpdateCallback(std::bind(&App::Update, this));
	engine.SetShutdownCallback(std::bind(&App::Shutdown, this));

	m_renderer = nullptr;
	m_window = nullptr;
	m_sceneManager = nullptr;
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
	
	m_sceneManager->CreateScene<GameScene>(L"GameScene");

}

void App::Update()
{
	UpdateWindow();
	m_sceneManager->Update(m_engine.GetDeltaTime());
	// Update your application here

	HandleInput();
}

void App::Shutdown()
{
	// Clean up your application here
}

void App::UpdateWindow()
{
	m_window->ProcessMessages();
	m_renderer->Update();
}

void App::HandleInput()
{
	if(Input::GetKey(Keyboard::A))
	{
		std::cout << "A key was pressed!" << std::endl;
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

void App::CreatePlayer()
{
	Entity* player = m_ecs->CreateEntity<Entity>();
	player->AddComponent<MeshRendererComponent>()->SetMesh(RessourceManager::Instance().GetSphere(), m_renderer);

	player->SetPosition(-2.0f, 0.0f, 5.0f);
}

void App::CreateDummyEntity()
{
	Entity* dummy = m_ecs->CreateEntity<Entity>();
	dummy->AddComponent<MeshRendererComponent>()->SetMesh(RessourceManager::Instance().GetCylinder(), m_renderer);
	dummy->SetPosition(2.0f, 0.0f, 5.0f);
	dummy->SetRotationX(XMConvertToRadians(45.0f));
}
