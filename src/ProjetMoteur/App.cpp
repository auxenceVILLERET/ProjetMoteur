#include "App.h"
#include <Window.h>
#include <Renderer.h>
#include <iostream>
#include "Engine/Engine.h"
#include "Core/InputsMethods.h"
#include "Engine/ECS/Components/CameraComponent.h"
#include "Engine/ECS/Systems/CameraSystem.h"

using namespace core;

App::App(Engine& engine) : m_engine(engine)
{
	engine.SetInitCallback(std::bind(&App::Initialize, this));
	engine.SetUpdateCallback(std::bind(&App::Update, this));
	engine.SetShutdownCallback(std::bind(&App::Shutdown, this));

	m_renderer = nullptr;
	m_window = nullptr;
}

void App::Initialize()
{
	// Initialize your application here
	m_window = new Window(800, 600, L"test");
	m_renderer = new Renderer();
	m_renderer->Initialize(m_window);
	m_ecs = &m_engine.GetECS();
	CreateCamera();

}

void App::Update()
{
	UpdateWindow();
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
	m_renderer->Render();
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

	m_cam->SetPosition(0.0f, 0.0f, -5.0f);

	m_cam->GetComponent<CameraComponent>()->SetAll(0.1f, 100.0f, 16.0f, 9.0f, true);
	
}
