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

#include "DescriptorHeapManager.h"

class Window;
class DxContext;
class SwapChainTargets;
class Pipeline;
class UploadContext;
class Entity;
class MeshRendererComponent;

using namespace DirectX;

class Renderer
{
public:
	Renderer();
	~Renderer();

	bool Initialize(Window* window, Entity* camera);
	void Shutdown();

	void Update();
	void Render(std::vector<MeshRendererComponent*> vMesh);
	void DrawUI(ID3D12GraphicsCommandList* cmdList);
	void DrawObj(MeshRendererComponent& obj);
	XMFLOAT4X4 BuildWorldViewProjMatrix(XMMATRIX& world);

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

	ID3D12Resource* m_texture = nullptr;
	ID3D12Resource* m_textureUpload = nullptr;
	DescriptorHandle m_textureSrv;

	DxContext* m_pDxContext = nullptr;
	SwapChainTargets* m_pSwapChainTargets = nullptr;
	Pipeline* m_pPipeline = nullptr;
	DescriptorHeapManager* m_pDescriptorHeapManager = nullptr;
	UploadContext* m_pUploadContext = nullptr;
};

#endif
