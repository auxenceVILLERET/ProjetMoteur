#pragma once

#include "Engine/Engine.h"

class App
{
public:
	App(Engine& engine);
	void Initialize();
	void Update();
	void Shutdown();
	void HandleInput();
private:
	Engine& m_engine;

};

