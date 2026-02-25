#pragma once

#include "Engine/Engine.h"

class Window;
class Renderer;
class SceneManager;
class Entity;
class ECS;
class Mesh;
class ResourceManager;

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

private:
	Engine& m_engine;
	Window* m_window;
	Renderer* m_renderer;
	SceneManager* m_sceneManager;
	ResourceManager* m_resourceManager;
	Entity* m_cam;
	ECS* m_ecs;
};

