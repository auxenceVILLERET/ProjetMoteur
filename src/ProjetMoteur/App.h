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

	void CreateSphere();
	void CreateCylinder();
	Entity* CreateCube();

	void CreateRail();
	void ProceduralRails();

	void Movement();

private:
	Engine& m_engine;
	Window* m_window = nullptr;
	Renderer* m_renderer = nullptr;
	Entity* m_cam = nullptr;
	ECS* m_ecs = nullptr;

	Entity* m_cube;
	Entity* m_sphere;
	
	Entity* m_rail;
	std::vector<Entity*> m_rails;

	float m_speedPlayer = 0.1f;
};

