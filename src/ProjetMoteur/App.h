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

	//TEST ENTITIES
	Entity* CreateSphere();
	Entity* CreateCylinder();
	Entity* CreateCube();
	Entity* CreateMoon();

	//RAILS
	void CreateRail();
	void ProceduralRails();

	//TEST CAMERA MOVING
	void Movement();

private:
	Engine& m_engine;
	Window* m_window = nullptr;
	Renderer* m_renderer = nullptr;
	Entity* m_cam = nullptr;
	ECS* m_ecs = nullptr;

	Entity* m_cube;
	Entity* m_sphere;
	Entity* m_cylinder;
	Entity* m_moon;
	
	Entity* m_rail;
	std::vector<Entity*> m_rails;

	float m_speedPlayer = 0.001f;
};

