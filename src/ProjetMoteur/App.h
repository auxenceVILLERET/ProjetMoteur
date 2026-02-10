#pragma once

#include "Engine/Engine.h"

class Window;
class Renderer;
class SceneManager;

class App
{
public:
	App(Engine& engine);

	void Initialize();
	void Update();
	void Shutdown();
	void UpdateWindow();

	void HandleInput();
private:
	Engine& m_engine;
	Window* m_window;
	Renderer* m_renderer;
	SceneManager* m_sceneManager;
};

