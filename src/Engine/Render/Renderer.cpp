#ifndef RENDERER_CPP_INCLUDED
#define RENDERER_CPP_INCLUDED

#include "Renderer.h"

#include "Window.h"

//Macro pour release mémoire
template<typename T>   
inline void SafeRelease(T*& p)
{
    if (p)
    {
        p->Release();
        p = nullptr;
    }
}

inline D3D12_CPU_DESCRIPTOR_HANDLE Offset(D3D12_CPU_DESCRIPTOR_HANDLE h, INT offsetInDescriptors, UINT descriptorSize)
{
    h.ptr += static_cast<SIZE_T>(offsetInDescriptors) * descriptorSize;
    return h;
}

Renderer::Renderer()
{
    m_screenViewport = { 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f };
    m_scissorRect = { 0, 0, 1, 1 };
    for (uint8_t i = 0; i < SwapChainBufferCount; ++i)
    {
        m_swapChainBuffer[i] = nullptr;
    }
}

Renderer::~Renderer()
{
    Shutdown();
}

bool Renderer::Initialize(HWND hwnd, uint32_t width, uint32_t height)
{
    m_width = width;
    m_height = height;

#if defined(DEBUG) || defined(_DEBUG)
    {
        ID3D12Debug* debugController = nullptr;
        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
        {
            debugController->EnableDebugLayer();
            debugController->Release();
        }
    }
#endif

    if (CreateDevice() == false)
        return false;

    if (CreateCommandObjects() == false)
        return false;

    if (CreateSwapChain(hwnd, width, height) == false)
        return false;

    if (CreateRtvDsvHeaps() == false)
        return false;

    CreateRenderTargets();
    CreateDepthStencil(width, height);

    m_screenViewport.TopLeftX = 0.0f;
    m_screenViewport.TopLeftY = 0.0f;
    m_screenViewport.Width = static_cast<float>(width);
    m_screenViewport.Height = static_cast<float>(height);
    m_screenViewport.MinDepth = 0.0f;
    m_screenViewport.MaxDepth = 1.0f;

    m_scissorRect.left = 0;
    m_scissorRect.top = 0;
    m_scissorRect.right = static_cast<LONG>(width);
    m_scissorRect.bottom = static_cast<LONG>(height);

    return true;
}

void Renderer::Shutdown()
{
    // Can be called multiple times safely.
    if (m_d3dDevice)
        WaitForGpu();

    // Release in roughly reverse creation order.
    SafeRelease(reinterpret_cast<IUnknown*&>(m_depthStencilBuffer));
    for (auto& b : m_swapChainBuffer)
        SafeRelease(reinterpret_cast<IUnknown*&>(b));

    SafeRelease(reinterpret_cast<IUnknown*&>(m_rtvHeap));
    SafeRelease(reinterpret_cast<IUnknown*&>(m_dsvHeap));

    SafeRelease(reinterpret_cast<IUnknown*&>(m_commandList));
    SafeRelease(reinterpret_cast<IUnknown*&>(m_directCmdListAlloc));
    SafeRelease(reinterpret_cast<IUnknown*&>(m_commandQueue));

    SafeRelease(reinterpret_cast<IUnknown*&>(m_fence));
    SafeRelease(reinterpret_cast<IUnknown*&>(m_swapChain));
    SafeRelease(reinterpret_cast<IUnknown*&>(m_d3dDevice));
    SafeRelease(reinterpret_cast<IUnknown*&>(m_dxgiFactory));
}

void Renderer::OnResize(uint32_t width, uint32_t height)
{
    if (m_d3dDevice == nullptr || m_swapChain == nullptr || m_directCmdListAlloc == nullptr)
        return;

    // Si minimizé, ne pas redimensionner.
    if (width == 0 || height == 0)
        return;

    // Flush before changing any resources.
    FlushCommandQueue();

    ThrowIfFailed(m_commandList->Reset(m_directCmdListAlloc, nullptr));

    m_width = width;
    m_height = height;

    WaitForGpu();

    // Release old buffers.
    for (int i = 0; i < SwapChainBufferCount; ++i)
    {
        m_swapChainBuffer[i]->Release();
        m_swapChainBuffer[i] = nullptr;
    }
        
    m_depthStencilBuffer->Release();

    // Resize swap chain.
    ThrowIfFailed(m_swapChain->ResizeBuffers(
        SwapChainBufferCount,
        width,
        height,
        m_backBufferFormat,
        DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));

    m_currBackBuffer = 0;

    CreateRenderTargets();
    CreateDepthStencil(width, height);

    m_screenViewport.TopLeftX = 0.0f;
    m_screenViewport.TopLeftY = 0.0f;
    m_screenViewport.Width = static_cast<float>(width);
    m_screenViewport.Height = static_cast<float>(height);
    m_screenViewport.MinDepth = 0.0f;
    m_screenViewport.MaxDepth = 1.0f;

    m_scissorRect.left = 0;
    m_scissorRect.top = 0;
    m_scissorRect.right = static_cast<LONG>(width);
    m_scissorRect.bottom = static_cast<LONG>(height);
}

void Renderer::Update(float dt)
{
}

void Renderer::Render()
{
    BeginFrame();
    EndFrame();
}

void Renderer::WaitForGpu()
{
    if (m_commandQueue == nullptr || m_fence == nullptr)
        return;

    ++m_currentFence;
    ThrowIfFailed(m_commandQueue->Signal(m_fence, m_currentFence));

    if (m_fence->GetCompletedValue() < m_currentFence)
    {
        HANDLE eventHandle = CreateEventEx(nullptr, nullptr, 0, EVENT_ALL_ACCESS);
        if (!eventHandle)
            throw std::runtime_error("CreateEventEx failed.");

        HRESULT hr = m_fence->SetEventOnCompletion(m_currentFence, eventHandle);
        if (FAILED(hr))
        {
            CloseHandle(eventHandle);
            throw std::runtime_error("SetEventOnCompletion failed.");
        }

        WaitForSingleObject(eventHandle, INFINITE);
        CloseHandle(eventHandle);
    }
}

bool Renderer::CreateDevice()
{
    ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&m_dxgiFactory)));

    // Try hardware device first.
    HRESULT hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_d3dDevice));
    if (FAILED(hr))
    {
        // Fallback to WARP.
        IDXGIAdapter* warpAdapter = nullptr;
        if (FAILED(m_dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter))))
            return false;

        hr = D3D12CreateDevice(warpAdapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_d3dDevice));
        warpAdapter->Release();
        if (FAILED(hr))
            return false;
    }

    // Fence.
    ThrowIfFailed(m_d3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)));

    m_rtvDescriptorSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    m_dsvDescriptorSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
    m_cbvSrvUavDescriptorSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

    return true;
}

bool Renderer::CreateCommandObjects()
{
    assert(m_d3dDevice);

    // Command queue.
    D3D12_COMMAND_QUEUE_DESC qdesc = {};
    qdesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    qdesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    ThrowIfFailed(m_d3dDevice->CreateCommandQueue(&qdesc, IID_PPV_ARGS(&m_commandQueue)));

    // Command allocator.
    ThrowIfFailed(m_d3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_directCmdListAlloc)));

    // Command list.
    ThrowIfFailed(m_d3dDevice->CreateCommandList(
        0,
        D3D12_COMMAND_LIST_TYPE_DIRECT,
        m_directCmdListAlloc,
        nullptr,
        IID_PPV_ARGS(&m_commandList)));

    // Start closed.
    m_commandList->Close();

    return true;
}

bool Renderer::CreateSwapChain(HWND hwnd, uint32_t width, uint32_t height)
{
    assert(m_dxgiFactory);
    assert(m_commandQueue);

    // Release previous swap chain if any.
	delete m_swapChain;

    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferDesc.Width = width;
    swapChainDesc.BufferDesc.Height = height;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.BufferDesc.Format = m_backBufferFormat;
    swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = SwapChainBufferCount;
    swapChainDesc.OutputWindow = hwnd;
    swapChainDesc.Windowed = TRUE;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    ThrowIfFailed(m_dxgiFactory->CreateSwapChain(
        m_commandQueue,
        &swapChainDesc,
        &m_swapChain));

    return true;
}

bool Renderer::CreateRtvDsvHeaps()
{
    assert(m_d3dDevice);

    // RTV heap.
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
    rtvHeapDesc.NumDescriptors = SwapChainBufferCount;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    ThrowIfFailed(m_d3dDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap)));

    // DSV heap.
    D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
    dsvHeapDesc.NumDescriptors = 1;
    dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    ThrowIfFailed(m_d3dDevice->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&m_dsvHeap)));

    return true;
}

void Renderer::CreateRenderTargets()
{
    assert(m_swapChain);
    assert(m_d3dDevice);
    assert(m_rtvHeap);

    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());
    for (UINT i = 0; i < SwapChainBufferCount; i++)
    {
        delete m_swapChainBuffer[i];
        ThrowIfFailed(m_swapChain->GetBuffer(i, IID_PPV_ARGS(&m_swapChainBuffer[i])));
        m_d3dDevice->CreateRenderTargetView(m_swapChainBuffer[i], nullptr, rtvHeapHandle);
        rtvHeapHandle.Offset(1, m_rtvDescriptorSize);
    }
}

void Renderer::CreateDepthStencil(uint32_t width, uint32_t height)
{
    assert(m_d3dDevice);
    assert(m_dsvHeap);

    delete m_depthStencilBuffer;

    D3D12_RESOURCE_DESC depthDesc = {};
    depthDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    depthDesc.Width = width;
    depthDesc.Height = height;
    depthDesc.DepthOrArraySize = 1;
    depthDesc.MipLevels = 1;
    depthDesc.Format = m_depthStencilFormat;
    depthDesc.SampleDesc.Count = 1;
    depthDesc.SampleDesc.Quality = 0;
    depthDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    depthDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

    D3D12_CLEAR_VALUE optClear = {};
    optClear.Format = m_depthStencilFormat;
    optClear.DepthStencil.Depth = 1.0f;
    optClear.DepthStencil.Stencil = 0;

    D3D12_HEAP_PROPERTIES heapProps = {};
    heapProps.Type = D3D12_HEAP_TYPE_DEFAULT;
    heapProps.CreationNodeMask = 1;
    heapProps.VisibleNodeMask = 1;

    ThrowIfFailed(m_d3dDevice->CreateCommittedResource(
        &heapProps,
        D3D12_HEAP_FLAG_NONE,
        &depthDesc,
        D3D12_RESOURCE_STATE_COMMON,
        &optClear,
        IID_PPV_ARGS(&m_depthStencilBuffer)));

    // Create DSV.
    D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
    dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
    dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Format = m_depthStencilFormat;
    dsvDesc.Texture2D.MipSlice = 0;

    m_d3dDevice->CreateDepthStencilView(
        m_depthStencilBuffer,
        &dsvDesc,
        m_dsvHeap->GetCPUDescriptorHandleForHeapStart());

    // Transition depth buffer to DEPTH_WRITE.
    ThrowIfFailed(m_directCmdListAlloc->Reset());

    ThrowIfFailed(m_commandList->Reset(m_directCmdListAlloc, nullptr));

    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Transition.pResource = m_depthStencilBuffer;
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COMMON;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_DEPTH_WRITE;
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    m_commandList->ResourceBarrier(1, &barrier);

    m_commandList->Close();
    ID3D12CommandList* lists[] = { m_commandList };
    m_commandQueue->ExecuteCommandLists(1, lists);
    WaitForGpu();
}

void Renderer::FlushCommandQueue()
{
    m_currentFence++;
    ThrowIfFailed(m_commandQueue->Signal(m_fence, m_currentFence));

    // Wait until the GPU has completed commands up to this fence point.
    if (m_fence->GetCompletedValue() < m_currentFence)
    {
        HANDLE eventHandle = CreateEventEx(nullptr, nullptr, false, EVENT_ALL_ACCESS);

        if (eventHandle == nullptr)
        {
            // Gérer l'erreur de création d'événement (par exemple, lancer une exception ou retourner)
            throw std::runtime_error("Échec de la création de l'événement pour la synchronisation GPU.");
        }

        // Fire event when GPU hits current fence.  
        ThrowIfFailed(m_fence->SetEventOnCompletion(m_currentFence, eventHandle));

        // Wait until the GPU hits current fence event is fired.
        WaitForSingleObject(eventHandle, INFINITE);
        CloseHandle(eventHandle);
    }
}

void Renderer::BeginFrame()
{
}

void Renderer::EndFrame()
{
}


#endif // !RENDERER_CPP_INCLUDED