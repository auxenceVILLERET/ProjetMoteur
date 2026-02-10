#ifndef RENDERER_CPP_INCLUDED
#define RENDERER_CPP_INCLUDED

#include "Renderer.h"

#include "Window.h"
#include "DxContext.h"
#include "SwapChainTargets.h"
#include "Pipeline.h"
#include "DescriptorHeapManager.h"
#include "UploadContext.h"

inline D3D12_CPU_DESCRIPTOR_HANDLE Offset(D3D12_CPU_DESCRIPTOR_HANDLE h, INT offsetInDescriptors, UINT descriptorSize)
{
    h.ptr += static_cast<SIZE_T>(offsetInDescriptors) * descriptorSize;
    return h;
}

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
    Shutdown();
}

bool Renderer::Initialize(Window* window)
{
    if (window == nullptr)
        return false;

    m_pWindow = window;

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

    // 1) DxContext (device + queue + cmdlist)
    m_pDxContext = new DxContext();
    if (m_pDxContext->CreateDevice() == false)
        return false;

    if (m_pDxContext->CreateCommandObjects() == false)
        return false;

    // 2) SwapChain + RTV/DSV + backbuffers
    m_pSwapChainTargets = new SwapChainTargets(this, window->GetWidth(), window->GetHeight());

    if (m_pSwapChainTargets->CreateSwapChain(window->GetHandle(), window->GetWidth(), window->GetHeight()) == false)
        return false;

    if (m_pSwapChainTargets->CreateRtvDsvHeaps() == false)
        return false;

    m_pSwapChainTargets->CreateRenderTargets();
    m_pSwapChainTargets->CreateDepthStencil(window->GetWidth(), window->GetHeight());

    // 3) Descriptor heap manager (CBV/SRV/UAV shader-visible)
    m_pDescriptorHeapManager = new DescriptorHeapManager();
    if (m_pDescriptorHeapManager->Initialize(
        m_pDxContext->GetDevice(),
        D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
        1024,
        true) == false)
    {
        return false;
    }

    // 4) Upload context
    m_pUploadContext = new UploadContext();
    if (m_pUploadContext->Initialize(m_pDxContext->GetDevice(), m_pDxContext->GetCommandQueue()) == false)
        return false;

    // 5) Pipeline (optionnel ici si tu n’as pas encore de shaders)
    //    -> Laisse-le commenté tant que tu n’as pas de fichiers HLSL.
    m_pPipeline = new Pipeline();
    if (CreateTestPipeline() == false) return false;
    if (CreateTestMesh() == false) return false;

    /*
    m_pPipeline = new Pipeline();

    std::vector<D3D12_INPUT_ELEMENT_DESC> layout = {
        // Exemple si tu as POSITION/COLOR :
        // { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,   D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        // { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    };

    bool ok = m_pPipeline->InitializeGraphics(
        m_pDxContext->GetDevice(),
        L"Shaders/MyShader.hlsl", "VSMain",
        L"Shaders/MyShader.hlsl", "PSMain",
        layout,
        DXGI_FORMAT_R8G8B8A8_UNORM,
        DXGI_FORMAT_D24_UNORM_S8_UINT,
        true
    );

    if (!ok) return false;
    */

    return true;
}

void Renderer::Shutdown()
{
    SafeRelease(m_ib);
    SafeRelease(m_vb);
    SafeRelease(m_ibUpload);
    SafeRelease(m_vbUpload);
    m_indexCount = 0;

    if (m_pDxContext)
        m_pDxContext->WaitForGpu();

    if (m_pPipeline)
    {
        m_pPipeline->Shutdown();
        delete m_pPipeline;
        m_pPipeline = nullptr;
    }

    if (m_pUploadContext)
    {
        m_pUploadContext->Shutdown();
        delete m_pUploadContext;
        m_pUploadContext = nullptr;
    }

    if (m_pDescriptorHeapManager)
    {
        m_pDescriptorHeapManager->Shutdown();
        delete m_pDescriptorHeapManager;
        m_pDescriptorHeapManager = nullptr;
    }

    if (m_pSwapChainTargets)
    {
        m_pSwapChainTargets->Shutdown();
        delete m_pSwapChainTargets;
        m_pSwapChainTargets = nullptr;
    }

    if (m_pDxContext)
    {
        m_pDxContext->Shutdown();
        delete m_pDxContext;
        m_pDxContext = nullptr;
    }

    m_pWindow = nullptr;
}

void Renderer::Update()
{
    if (m_pWindow)
    {
        if (m_pWindow->IsResizing())
			m_pSwapChainTargets->Resize(m_pWindow->GetWidth(), m_pWindow->GetHeight());
    }
}

void Renderer::Render()
{
    if (m_pDxContext == nullptr || m_pSwapChainTargets == nullptr) return;
    if (m_pWindow && m_pWindow->IsMinimized()) return;

    BeginFrame();

    // draw calls (PSO, root signature, IA, DrawInstanced...)*
    ID3D12GraphicsCommandList* cmd = m_pDxContext->GetCommandList();

    cmd->SetPipelineState(m_pPipeline->GetPSO());
    cmd->SetGraphicsRootSignature(m_pPipeline->GetRootSignature());

    cmd->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    cmd->IASetVertexBuffers(0, 1, &m_vbView);
    cmd->IASetIndexBuffer(&m_ibView);

    cmd->DrawIndexedInstanced(m_indexCount, 1, 0, 0, 0);

    EndFrame();
}

bool Renderer::CreateTestPipeline()
{
    std::vector<D3D12_INPUT_ELEMENT_DESC> layout =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,
          D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },

        { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12,
          D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    };

    // Formats identiques à SwapChainTargets (R8G8B8A8 + D24S8)
    return m_pPipeline->InitializeGraphics(
        m_pDxContext->GetDevice(),
        L"../../src/Render/Simple.hlsl", "VSMain",
        L"../../src/Render/Simple.hlsl", "PSMain",
        layout,
        DXGI_FORMAT_R8G8B8A8_UNORM,
        DXGI_FORMAT_D24_UNORM_S8_UINT,
        true
    );
}

bool Renderer::CreateTestMesh()
{
    // Triangle en clip-space (pas besoin de matrices)
    Vertex vertices[] =
    {
        { { 0.0f,  0.5f, 0.0f }, { 1, 0, 0, 1 } },
        { { 0.5f, -0.5f, 0.0f }, { 0, 1, 0, 1 } },
        { { -0.5f,-0.5f, 0.0f }, { 0, 0, 1, 1 } },
    };

    uint16_t indices[] = { 0, 1, 2 };
    m_indexCount = 3;

    m_pUploadContext->Begin();

    // VB
    {
        const uint64_t vbBytes = sizeof(vertices);
        if (m_pUploadContext->UploadBuffer(vertices, vbBytes, m_vb, m_vbUpload, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER) == false)
            return false;

        m_vbView.BufferLocation = m_vb->GetGPUVirtualAddress();
        m_vbView.SizeInBytes = (UINT)vbBytes;
        m_vbView.StrideInBytes = sizeof(Vertex);
    }

    // IB
    {
        const uint64_t ibBytes = sizeof(indices);
        if (!m_pUploadContext->UploadBuffer(indices, ibBytes, m_ib, m_ibUpload, D3D12_RESOURCE_STATE_INDEX_BUFFER))
            return false;

        m_ibView.BufferLocation = m_ib->GetGPUVirtualAddress();
        m_ibView.SizeInBytes = (UINT)ibBytes;
        m_ibView.Format = DXGI_FORMAT_R16_UINT;
    }

    m_pUploadContext->EndAndWait();

    // Après EndAndWait, tu peux libérer les upload buffers si tu veux
    SafeRelease(m_vbUpload);
    SafeRelease(m_ibUpload);

    return true;
}

void Renderer::BeginFrame()
{
	ID3D12Device* pDevice = m_pDxContext->GetDevice();
	ID3D12CommandQueue* pCommandQueue = m_pDxContext->GetCommandQueue();
	ID3D12GraphicsCommandList* pCommandList = m_pDxContext->GetCommandList();
	ID3D12CommandAllocator* pCommandAllocator = m_pDxContext->GetCommandAllocator();

	assert(pDevice);
    assert(pCommandQueue);
    assert(pCommandList);
    assert(pCommandAllocator);

    // Reset allocator and list.
    HRESULT hr = pCommandAllocator->Reset();
    if (FAILED(hr))
		throw std::runtime_error("Command allocator reset failed.");

    hr = pCommandList->Reset(pCommandAllocator, nullptr);
	if (FAILED(hr))
		throw std::runtime_error("Command list reset failed.");

    // Set viewport/scissor.
    pCommandList->RSSetViewports(1, m_pSwapChainTargets->GetScreenViewport());
    pCommandList->RSSetScissorRects(1, m_pSwapChainTargets->GetScissorRect());

    // RTV handle
    D3D12_CPU_DESCRIPTOR_HANDLE rtv = m_pSwapChainTargets->GetRtvHeap()->GetCPUDescriptorHandleForHeapStart();
    rtv = Offset(rtv, m_pSwapChainTargets->GetCurrentBackBufferIndex(), m_pDxContext->GetRtvDescriptorSize());

    // DSV handle
    D3D12_CPU_DESCRIPTOR_HANDLE dsv = m_pSwapChainTargets->GetDsvHeap()->GetCPUDescriptorHandleForHeapStart();

    // Transition current back buffer: PRESENT -> RENDER_TARGET.
    ID3D12Resource* backBuffer = m_pSwapChainTargets->GetCurrentBackBuffer();
    D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(backBuffer,
        D3D12_RESOURCE_STATE_PRESENT,
        D3D12_RESOURCE_STATE_RENDER_TARGET);
    pCommandList->ResourceBarrier(1, &barrier);

    pCommandList->OMSetRenderTargets(1, &rtv, true, &dsv);

    // Clear.
    float clearColor[4] = { 0.08f, 0.10f, 0.14f, 1.0f };
    pCommandList->ClearRenderTargetView(rtv, clearColor, 0, nullptr);
    pCommandList->ClearDepthStencilView(dsv, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);
}

void Renderer::EndFrame()
{
    ID3D12CommandQueue* pCommandQueue = m_pDxContext->GetCommandQueue();
    ID3D12GraphicsCommandList* pCommandList = m_pDxContext->GetCommandList();
    ID3D12CommandAllocator* pCommandAllocator = m_pDxContext->GetCommandAllocator();

    // Transition back buffer: RENDER_TARGET -> PRESENT.
    ID3D12Resource* backBuffer = m_pSwapChainTargets->GetCurrentBackBuffer();
    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Transition.pResource = backBuffer;
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    pCommandList->ResourceBarrier(1, &barrier);

    HRESULT hr = pCommandList->Close();
	if (FAILED(hr))
		throw std::runtime_error("Command list close failed.");

    ID3D12CommandList* lists[] = { pCommandList };
    pCommandQueue->ExecuteCommandLists(1, lists);

    // Present.
    hr = m_pSwapChainTargets->GetSwapChain()->Present(1, 0);
	if (FAILED(hr))
		throw std::runtime_error("Swap chain present failed.");

    // Simple but safe: CPU waits GPU each frame.
    m_pDxContext->WaitForGpu();

    m_pSwapChainTargets->UpdateCurrentBackBuffer();
}

#endif // !RENDERER_CPP_INCLUDED