#pragma once

#include "Engine/Core/Engine.h"

class App
{
public:
	App(Engine& engine);
	void Initialize();
	void Update();
	void Shutdown();

private:
	Engine& m_engine;
};

