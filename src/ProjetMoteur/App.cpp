#include "App.h"

#include <iostream>
#include "Engine/Engine.h"
#include "Core/InputsMethods.h"

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
}

void App::Update()
{
	// Update your application here
	HandleInput();
}

void App::Shutdown()
{
	// Clean up your application here
}

void App::HandleInput()
{
	if(Input::GetKeyDown(Keyboard::SPACE))
	{
		std::cout << "A key was pressed!" << std::endl;
	}
}
