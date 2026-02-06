#ifndef RENDERER_CPP_INCLUDED
#define RENDERER_CPP_INCLUDED

#include "Renderer.h"

#include "Window.h"
#include "DxContext.h"
#include "SwapChainTargets.h"

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
	uint32_t width = window->GetWidth();
    uint32_t height = window->GetHeight();

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

	m_pDxContext = new DxContext();
    if (m_pDxContext->CreateDevice() == false)
        return false;
    if (m_pDxContext->CreateCommandObjects() == false)
        return false;

	m_pSwapChainTargets = new SwapChainTargets(this, width, height);
    if (m_pSwapChainTargets->CreateSwapChain(window->GetHandle(), width, height) == false)
        return false;
    if (m_pSwapChainTargets->CreateRtvDsvHeaps() == false)
        return false;

    m_pSwapChainTargets->CreateRenderTargets();
    m_pSwapChainTargets->CreateDepthStencil(width, height);

    return true;
}

void Renderer::Shutdown()
{
    // Can be called multiple times safely.
    if (m_pDxContext->GetDevice())
        m_pDxContext->WaitForGpu();

    // Release in roughly reverse creation order.
    //SafeRelease(m_depthStencilBuffer);
    //for (auto& b : m_swapChainBuffer) 
    //    SafeRelease(b);
    //SafeRelease(m_rtvHeap);
    //SafeRelease(m_dsvHeap);
    //SafeRelease(m_commandList);
    //SafeRelease(m_directCmdListAlloc);
    //SafeRelease(m_commandQueue);
    //SafeRelease(m_fence);
    //SafeRelease(m_swapChain);
    //SafeRelease(m_d3dDevice);
    //SafeRelease(m_dxgiFactory);
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
    BeginFrame();
    EndFrame();
}


void Renderer::BeginFrame()
{
	ID3D12CommandQueue* pCommandQueue = m_pDxContext->GetCommandQueue();
	ID3D12GraphicsCommandList* pCommandList = m_pDxContext->GetCommandList();
	ID3D12CommandAllocator* pCommandAllocator = m_pDxContext->GetCommandAllocator();

    assert(pCommandQueue);
    assert(pCommandList);
    assert(pCommandAllocator);

    // Reset allocator and list.
    HRESULT hr = m_pDxContext->GetCommandAllocator()->Reset();
    if (FAILED(hr))
		throw std::runtime_error("Command allocator reset failed.");

    hr = m_pDxContext->GetCommandList()->Reset(m_pDxContext->GetCommandAllocator(), nullptr);
	if (FAILED(hr))
		throw std::runtime_error("Command list reset failed.");

    // Transition current back buffer: PRESENT -> RENDER_TARGET.
    ID3D12Resource* backBuffer = m_pSwapChainTargets->GetCurrentBackBuffer();
    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Transition.pResource = backBuffer;
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    pCommandList->ResourceBarrier(1, &barrier);

    // Set viewport/scissor.
    pCommandList->RSSetViewports(1, &m_pSwapChainTargets->GetScreenViewport());
    pCommandList->RSSetScissorRects(1, &m_pSwapChainTargets->GetScissorRect());

    // RTV handle for current back buffer.
    D3D12_CPU_DESCRIPTOR_HANDLE rtv = m_pSwapChainTargets->GetRtvHeap()->GetCPUDescriptorHandleForHeapStart();
    rtv = Offset(rtv, m_pSwapChainTargets->GetCurrentBackBufferIndex(), m_pDxContext->GetRtvDescriptorSize());

    D3D12_CPU_DESCRIPTOR_HANDLE dsv = m_pSwapChainTargets->GetDsvHeap()->GetCPUDescriptorHandleForHeapStart();
    pCommandList->OMSetRenderTargets(1, &rtv, true, &dsv);

    // Clear.
    const float clearColor[] = { 0.07f, 0.07f, 0.12f, 1.0f };
    pCommandList->ClearRenderTargetView(rtv, clearColor, 0, nullptr);
    pCommandList->ClearDepthStencilView(dsv, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);
}

void Renderer::EndFrame()
{
    ID3D12Resource* backBuffer = m_pSwapChainTargets->GetCurrentBackBuffer();

    // Transition back buffer: RENDER_TARGET -> PRESENT.
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

    m_pSwapChainTargets->SetCurrentBackBufferIndex((m_pSwapChainTargets->GetCurrentBackBufferIndex() + 1) % SwapChainBufferCount);
}

#endif // !RENDERER_CPP_INCLUDED