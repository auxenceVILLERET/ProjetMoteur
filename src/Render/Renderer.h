#ifndef RENDERER_H_DEFINED
#define RENDERER_H_DEFINED

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include "Helpers/d3dUtil.h"

// Link necessary d3d12 libraries.
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

class Window;
class DxContext;
class SwapChainTargets;

class Renderer
{
public:
	Renderer();
	~Renderer();

	bool Initialize(Window* window);
	void Shutdown();

	void Update();
	void Render();

	Window* GetWindow() const { return m_pWindow; }
	DxContext* GetDxContext() const { return m_pDxContext; }
	SwapChainTargets* GetSwapChainTargets() const { return m_pSwapChainTargets; }

private:	
	template<typename T>
    void SafeRelease(T*& p);

    void BeginFrame();
    void EndFrame();

	Window* m_pWindow = nullptr;

	DxContext* m_pDxContext = nullptr;
	SwapChainTargets* m_pSwapChainTargets = nullptr;

};

#endif
