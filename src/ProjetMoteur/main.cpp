#include <windows.h>
#include <iostream>
#include "main.h"

#include "Engine/Engine.h"
#include "App.h"


void AttachConsoleIO()
{
	AllocConsole();

	FILE* f;
	freopen_s(&f, "CONIN$", "r", stdin);
	freopen_s(&f, "CONOUT$", "w", stdout);
	freopen_s(&f, "CONOUT$", "w", stderr);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	AttachConsoleIO();

	Engine engine;
	
	App app(engine);

	engine.Initialize();
	engine.Run();
	engine.Shutdown();

}

