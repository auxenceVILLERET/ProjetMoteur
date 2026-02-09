#include "App.h"
#include <Window.h>
#include <Renderer.h>
#include <iostream>
#include "Engine/Engine.h"

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
	m_renderer = new Renderer();
	m_renderer->Initialize(m_window);
}

void App::Update()
{
	// Update your application here

	m_renderer->Update();
	m_renderer->Render();

}

void App::Shutdown()
{
	// Clean up your application here
}
