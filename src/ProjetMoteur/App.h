#pragma once

#include "Engine/Engine.h"

class Window;
class Renderer;
class Entity;
class ECS;
class Mesh;

class App
{
public:
	App(Engine& engine);

	void Initialize();
	void Update();
	void Shutdown();
	void UpdateWindow();

	void HandleInput();
	void CreateCamera();

	void CreatePlayer();
	void CreateDummyEntity();
private:
	Engine& m_engine;
	Window* m_window = nullptr;
	Renderer* m_renderer = nullptr;
	Entity* m_cam = nullptr;
	ECS* m_ecs = nullptr;
};

