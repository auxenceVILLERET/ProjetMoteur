#include "main.h"

#include <iostream>
#include <windows.h>
#include "Engine/Render/Window.h"
#include "Engine/Render/Renderer.h"

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nCmdShow)
{
    AllocConsole();

    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    freopen_s(&f, "CONOUT$", "w", stderr);
    freopen_s(&f, "CONIN$", "r", stdin);

    Window window(800, 600, L"test");
	Renderer renderer;
	renderer.Initialize(window.GetHandle(), 800, 600);
     
    while (window.ProcessMessages())
    {
        renderer.Update(0.0f);
        renderer.Render();
    }

    return 0;
}