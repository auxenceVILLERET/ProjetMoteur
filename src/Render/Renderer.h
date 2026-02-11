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
class Pipeline;
class DescriptorHeapManager;
class UploadContext;
class Mesh;
class Entity;

using namespace DirectX;

class Renderer
{
public:
	Renderer();
	~Renderer();

	bool Initialize(Window* window, Entity* camera);
	void Shutdown();

	void Update();
	void Render();

	bool CreateTestMesh();

	Window* GetWindow() const { return m_pWindow; }
	DxContext* GetDxContext() const { return m_pDxContext; }
	SwapChainTargets* GetSwapChainTargets() const { return m_pSwapChainTargets; }
	Pipeline* GetPipeline() const { return m_pPipeline; }
	DescriptorHeapManager* GetDescriptorHeapManager() const { return m_pDescriptorHeapManager; }
	UploadContext* GetUploadContext() const { return m_pUploadContext; }

private:	
    void BeginFrame();
    void EndFrame();

	Window* m_pWindow = nullptr;
	Entity* m_pCamera = nullptr;

	std::vector<Mesh> m_vMeshes;

	DxContext* m_pDxContext = nullptr;
	SwapChainTargets* m_pSwapChainTargets = nullptr;
	Pipeline* m_pPipeline = nullptr;
	DescriptorHeapManager* m_pDescriptorHeapManager = nullptr;
	UploadContext* m_pUploadContext = nullptr;

	float angle = 0.f;
};

#endif
