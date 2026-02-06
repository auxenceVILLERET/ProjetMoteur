#include "App.h"

#include <iostream>
#include "ECS/ECS.h"
#include "Engine/ECS/MovementSystem.h"
#include "Engine/ECS/PositionComponent.h"
#include "Engine/ECS/VelocityComponent.h"

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
}

void App::Shutdown()
{
	// Clean up your application here
}
