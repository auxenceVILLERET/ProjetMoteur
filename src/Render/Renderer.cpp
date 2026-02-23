#ifndef RENDERER_CPP_INCLUDED
#define RENDERER_CPP_INCLUDED

#include "Renderer.h"

#include <wincodec.h>
#pragma comment(lib, "windowscodecs.lib")

#include "Window.h"
#include "DxContext.h"
#include "SwapChainTargets.h"
#include "Pipeline.h"
#include "DescriptorHeapManager.h"
#include "UploadContext.h"
#include "Engine/Mesh.h"
#include "Engine/ECS/Entity.h"
#include "Engine/ECS/Components/CameraComponent.h"
#include "Engine/ECS/Components/MeshRendererComponent.h"

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

bool Renderer::Initialize(Window* window, Entity* camera)
{
    if (window == nullptr || camera == nullptr)
        return false;

    m_pWindow = window;
    m_pCamera = camera;

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
    if (m_pDescriptorHeapManager->Initialize(m_pDxContext->GetDevice(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 1024, true) == false)
        return false;

    // 4) Upload context
    m_pUploadContext = new UploadContext();
    if (m_pUploadContext->Initialize(m_pDxContext->GetDevice(), m_pDxContext->GetCommandQueue()) == false)
        return false;

    // 5) Pipeline
    m_pPipeline = new Pipeline();
    if (m_pPipeline->InitializePipeline(m_pDxContext->GetDevice()) == false)
		return false;

    if (CreateTestTexture() == false)
		return false;

    return true;
}

void Renderer::Shutdown()
{

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

	SafeRelease(m_texture);
	SafeRelease(m_textureUpload);

    m_pWindow = nullptr;
}

void Renderer::Update()
{
    if (m_pWindow)
    {
        if (m_pWindow->IsResizing())
        {
            m_pSwapChainTargets->Resize(m_pWindow->GetWidth(), m_pWindow->GetHeight());
			m_pCamera->GetComponent<CameraComponent>()->SetWindowSize(
                static_cast<float>(m_pWindow->GetWidth()),
                static_cast<float>(m_pWindow->GetHeight())
            );
        }
    }
}

void Renderer::Render(std::vector<MeshRendererComponent*> vObj)
{
    if (m_pDxContext == nullptr || m_pSwapChainTargets == nullptr) return;
    if (m_pWindow && m_pWindow->IsMinimized()) return;

    BeginFrame();

    // draw calls (PSO, root signature, IA, DrawInstanced...)*
    ID3D12GraphicsCommandList* cmd = m_pDxContext->GetCommandList();

    cmd->SetPipelineState(m_pPipeline->GetPSO());
    cmd->SetGraphicsRootSignature(m_pPipeline->GetRootSignature());

    ID3D12DescriptorHeap* heaps = m_pDescriptorHeapManager->GetHeap();
    cmd->SetDescriptorHeaps(1, &heaps);
    if (m_textureSrv.IsValid())
    {
        cmd->SetGraphicsRootDescriptorTable(1, m_textureSrv.gpu);
	}
    cmd->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    for (MeshRendererComponent* m : vObj)
    {
        DrawObj(*m);
    }

    EndFrame();
}

void Renderer::DrawObj(MeshRendererComponent& obj)
{
    // root param 0 = CBV(b0)
    ID3D12GraphicsCommandList* cmd = m_pDxContext->GetCommandList();
    cmd->SetGraphicsRootConstantBufferView(0, obj.GetCbAddress());
    obj.GetMesh()->Draw(cmd);
}

bool Renderer::LoadWIC_RGBA8(const wchar_t* filename, std::vector<uint8_t>& outRGBA, uint32_t& outW, uint32_t& outH)
{
    outRGBA.clear(); outW = outH = 0;

    IWICImagingFactory* factory = nullptr;
    IWICBitmapDecoder* decoder = nullptr;
    IWICBitmapFrameDecode* frame = nullptr;
    IWICFormatConverter* converter = nullptr;

    HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    (void)hr; // ok si déjà init ailleurs

    hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&factory));
    if (FAILED(hr)) return false;

    hr = factory->CreateDecoderFromFilename(filename, nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder);
    if (FAILED(hr)) 
    { 
        factory->Release();
        return false;
    }

    hr = decoder->GetFrame(0, &frame);
    if (FAILED(hr)) 
    { 
        decoder->Release(); 
        factory->Release(); 
        return false;
    }

    UINT w = 0, h = 0;
    frame->GetSize(&w, &h);
    outW = (uint32_t)w;
    outH = (uint32_t)h;

    hr = factory->CreateFormatConverter(&converter);
    if (FAILED(hr)) 
    { 
        frame->Release(); 
        decoder->Release(); 
        factory->Release(); 
        return false; 
    }

    hr = converter->Initialize(frame, GUID_WICPixelFormat32bppRGBA,
        WICBitmapDitherTypeNone, nullptr, 0.0, WICBitmapPaletteTypeCustom);
    if (FAILED(hr))
    { 
        converter->Release();
        frame->Release();
        decoder->Release(); 
        factory->Release(); 
        return false; 
    }

    outRGBA.resize((size_t)outW * (size_t)outH * 4);
    hr = converter->CopyPixels(nullptr, outW * 4, (UINT)outRGBA.size(), outRGBA.data());

    converter->Release();
    frame->Release();
    decoder->Release();
    factory->Release();

    return SUCCEEDED(hr);
}

bool Renderer::CreateTestTexture()
{
    ID3D12Device* device = m_pDxContext->GetDevice();
    UploadContext* up = m_pUploadContext;

    if (device == nullptr || up == nullptr) return false;

    // 1) Load image (RGBA8)
    std::vector<uint8_t> rgba;
    uint32_t w = 0, h = 0;
    if (LoadWIC_RGBA8(L"../../res/testTexture.png", rgba, w, h) == false)
        return false;

    // 2) Create default texture
    SafeRelease(m_texture);
    SafeRelease(m_textureUpload);

    D3D12_HEAP_PROPERTIES defaultHeap = {};
    defaultHeap.Type = D3D12_HEAP_TYPE_DEFAULT;
    defaultHeap.CreationNodeMask = 1;
    defaultHeap.VisibleNodeMask = 1;

    D3D12_RESOURCE_DESC texDesc = {};
    texDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    texDesc.Alignment = 0;
    texDesc.Width = w;
    texDesc.Height = h;
    texDesc.DepthOrArraySize = 1;
    texDesc.MipLevels = 1;
    texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    texDesc.SampleDesc.Count = 1;
    texDesc.SampleDesc.Quality = 0;
    texDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    texDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

    HRESULT hr = device->CreateCommittedResource(
        &defaultHeap,
        D3D12_HEAP_FLAG_NONE,
        &texDesc,
        D3D12_RESOURCE_STATE_COPY_DEST,
        nullptr,
        IID_PPV_ARGS(&m_texture)
    );
    if (FAILED(hr))
        return false;

    // 3) Create upload buffer for texture (footprints)
    UINT64 uploadSize = 0;
    D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint = {};
    UINT numRows = 0;
    UINT64 rowSizeInBytes = 0;

    device->GetCopyableFootprints(&texDesc, 0, 1, 0, &footprint, &numRows, &rowSizeInBytes, &uploadSize);

    D3D12_HEAP_PROPERTIES uploadHeap = {};
    uploadHeap.Type = D3D12_HEAP_TYPE_UPLOAD;
    uploadHeap.CreationNodeMask = 1;
    uploadHeap.VisibleNodeMask = 1;

    D3D12_RESOURCE_DESC bufDesc = {};
    bufDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    bufDesc.Width = uploadSize;
    bufDesc.Height = 1;
    bufDesc.DepthOrArraySize = 1;
    bufDesc.MipLevels = 1;
    bufDesc.Format = DXGI_FORMAT_UNKNOWN;
    bufDesc.SampleDesc.Count = 1;
    bufDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    hr = device->CreateCommittedResource(
        &uploadHeap,
        D3D12_HEAP_FLAG_NONE,
        &bufDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&m_textureUpload)
    );
    if (FAILED(hr)) 
        return false;

    // 4) Copy pixels into upload buffer respecting RowPitch
    uint8_t* mapped = nullptr;
    D3D12_RANGE r = { 0, 0 };
    hr = m_textureUpload->Map(0, &r, (void**)&mapped);

    if (FAILED(hr)) 
        return false;

    const uint8_t* src = rgba.data();
    uint8_t* dst = mapped + footprint.Offset;

    const uint32_t srcRowPitch = w * 4;
    for (uint32_t y = 0; y < h; ++y)
    {
        std::memcpy(dst + y * footprint.Footprint.RowPitch, src + y * srcRowPitch, srcRowPitch);
    }

    m_textureUpload->Unmap(0, nullptr);

    // 5) Record copy commands
    up->Begin();
    ID3D12GraphicsCommandList* cmd = up->GetCommandList();

    D3D12_TEXTURE_COPY_LOCATION dstLoc = {};
    dstLoc.pResource = m_texture;
    dstLoc.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
    dstLoc.SubresourceIndex = 0;

    D3D12_TEXTURE_COPY_LOCATION srcLoc = {};
    srcLoc.pResource = m_textureUpload;
    srcLoc.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
    srcLoc.PlacedFootprint = footprint;

    cmd->CopyTextureRegion(&dstLoc, 0, 0, 0, &srcLoc, nullptr);

    up->Transition(m_texture, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
    up->EndAndWait();

    // 6) Create SRV in your shader-visible CBV/SRV/UAV heap
    if (m_textureSrv.IsValid() == false)
        m_textureSrv = m_pDescriptorHeapManager->Allocate();
    if (m_textureSrv.IsValid() == false)
        return false;

    D3D12_SHADER_RESOURCE_VIEW_DESC srv = {};
    srv.Format = texDesc.Format;
    srv.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srv.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srv.Texture2D.MipLevels = 1;

    device->CreateShaderResourceView(m_texture, &srv, m_textureSrv.cpu);

    SafeRelease(m_textureUpload);

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

XMFLOAT4X4 Renderer::BuildWorldViewProjMatrix(XMMATRIX& world)
{
    XMMATRIX view = XMLoadFloat4x4(&m_pCamera->GetComponent<CameraComponent>()->GetViewMatrix());
    XMMATRIX proj = XMLoadFloat4x4(&m_pCamera->GetComponent<CameraComponent>()->GetProjectionMatrix());
    XMMATRIX wvp = world * view * proj;
    wvp = XMMatrixTranspose(wvp);

    XMFLOAT4X4 wvpFloat4x4;
    XMStoreFloat4x4(&wvpFloat4x4, wvp);
    return wvpFloat4x4;
}

#endif // !RENDERER_CPP_INCLUDED