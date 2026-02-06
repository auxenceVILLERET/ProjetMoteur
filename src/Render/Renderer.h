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

class Renderer
{
public:
	Renderer();
	~Renderer();

	bool Initialize(Window* window);
	void Shutdown();

	void OnResize();
	void Update();
	void Render();

	void WaitForGpu();

private:
    bool CreateDevice();
    bool CreateCommandObjects();
    bool CreateSwapChain(HWND hwnd, uint32_t width, uint32_t height);
    bool CreateRtvDsvHeaps();
    void CreateRenderTargets();
    void CreateDepthStencil(uint32_t width, uint32_t height);

    void FlushCommandQueue();

	template<typename T>
    void SafeRelease(T*& p);

    void BeginFrame();
    void EndFrame();

	Window* m_pWindow = nullptr;

    IDXGIFactory4* m_dxgiFactory = nullptr;
    IDXGISwapChain* m_swapChain = nullptr;
    ID3D12Device* m_d3dDevice = nullptr;

    ID3D12Fence* m_fence = nullptr;
    UINT64 m_currentFence = 0;

    ID3D12CommandQueue* m_commandQueue = nullptr;
    ID3D12CommandAllocator* m_directCmdListAlloc = nullptr;
    ID3D12GraphicsCommandList* m_commandList = nullptr;

    static const uint8_t SwapChainBufferCount = 2;
    uint8_t m_currBackBuffer = 0;
    ID3D12Resource* m_swapChainBuffer[SwapChainBufferCount];
    ID3D12Resource* m_depthStencilBuffer = nullptr;

    ID3D12DescriptorHeap* m_rtvHeap = nullptr;
    ID3D12DescriptorHeap* m_dsvHeap = nullptr;

    D3D12_VIEWPORT m_screenViewport;
    D3D12_RECT m_scissorRect;

    UINT m_rtvDescriptorSize = 0;
    UINT m_dsvDescriptorSize = 0;
    UINT m_cbvSrvUavDescriptorSize = 0;

    // Derived class should set these in derived constructor to customize starting values.
    std::wstring m_mainWindowCaption = L"Window";
    D3D_DRIVER_TYPE m_d3dDriverType = D3D_DRIVER_TYPE_HARDWARE;
    DXGI_FORMAT m_backBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
    DXGI_FORMAT m_depthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

    uint32_t m_width = 0;
    uint32_t m_height = 0;
};

#include "Renderer.inl"

#endif
