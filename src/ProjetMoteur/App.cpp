#include "App.h"
#include <Window.h>
#include <Renderer.h>
#include <iostream>
#include "Engine/Engine.h"
#include "Core/InputsMethods.h"
#include "Engine/Scene/SceneManager.h"

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
	m_renderer = new Renderer();
	m_renderer->Initialize(m_window);
	m_sceneManager = new SceneManager();
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
