#include "App.h"
#include <Window.h>
#include <Renderer.h>
#include <iostream>
#include "Engine/Engine.h"
#include "Core/InputsMethods.h"
#include "Engine/ECS/Components/CameraComponent.h"
#include "Engine/ECS/Systems/CameraSystem.h"
#include "Engine/RessourceManager.h"
#include "Engine/ECS/Components/MeshRendererComponent.h"
#include "Render/UploadContext.h" 
#include "Mesh.h"
#include "Engine/ECS/Systems/RenderSystem.h"

using namespace core;

App::App(Engine& engine) : m_engine(engine)
{
	engine.SetInitCallback(std::bind(&App::Initialize, this));
	engine.SetUpdateCallback(std::bind(&App::Update, this));
	engine.SetShutdownCallback(std::bind(&App::Shutdown, this));
}

void App::Initialize()
{
	// Initialize your application here
	m_window = new Window(800, 600, L"test");
	m_ecs = &m_engine.GetECS();
	CreateCamera();
	m_renderer = new Renderer();
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

	CreateSphere();
	CreateCylinder();
	m_cube = CreateCube();
}

void App::Update()
{
	UpdateWindow();
	// Update your application here
	
	ProceduralRails();
	HandleInput();

	//Test on entities
	m_cube->SetRotationX(XMConvertToRadians(.1f));
	m_cube->SetRotationY(XMConvertToRadians(.1f));
	m_cube->SetRotationZ(XMConvertToRadians(.1f));

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
}

void App::HandleInput()
{
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

// TEST ENTITIES
void App::CreateSphere()
{
	Entity* player = m_ecs->CreateEntity<Entity>();
	player->AddComponent<MeshRendererComponent>()->SetMesh(RessourceManager::Instance().GetSphere(), m_renderer);

	player->SetPosition(-2.0f, 0.0f, 5.0f);
}

void App::CreateCylinder()
{
	Entity* dummy = m_ecs->CreateEntity<Entity>();
	dummy->AddComponent<MeshRendererComponent>()->SetMesh(RessourceManager::Instance().GetCylinder(), m_renderer);
	dummy->SetPosition(2.0f, 0.0f, 5.0f);
	dummy->SetRotationX(XMConvertToRadians(45.0f));
}

Entity* App::CreateCube() {
	Entity* cube = m_ecs->CreateEntity<Entity>();
	cube->AddComponent<MeshRendererComponent>()->SetMesh(RessourceManager::Instance().GetCube(), m_renderer);
	cube->SetPosition(0.0f, 1.0f, 5.0f);
	cube->SetScale(.5f);
	return cube;
}
///////////////////

// RAIL GENERATION
void App::CreateRail() 
{
	m_rail = m_ecs->CreateEntity<Entity>();
	m_rail->AddComponent<MeshRendererComponent>()->SetMesh(RessourceManager::Instance().GetCylinder(), m_renderer);
	m_rail->SetScaleVector({ .1f, 3.0f, .1f });
	m_rail->SetRotationX(XM_PIDIV2);
}

void App::ProceduralRails()
{
	if (Input::GetKeyDown(Keyboard::SPACE)) {
		CreateRail();
		m_rail->SetPosition(0.0f, -1.0f, m_rail->GetScale().y * m_rails.size());
		m_rails.push_back(m_rail);
		std::cout << " newrail " << std::endl;
	}
}
///////////////////

// MOVEMENT OF THE CAMERA
void App::Movement()
{
	if (Input::GetKey(Keyboard::Z)) {
		m_cam->MoveForward(1.0f * m_speedPlayer);
		// WorldMatrix is not updated
	}
}
