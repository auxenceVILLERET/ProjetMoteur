#ifndef UI_RENDERER_CPP_INCLUDED
#define UI_RENDERER_CPP_INCLUDED

#include "UIRender.h"

#include "DxContext.h" 
#include "UploadContext.h"
#include "Pipeline.h"
#include "Texture2D.h"

bool UIRender::Initialize(DxContext* dx, UploadContext* uploader, DescriptorHeapManager* heap, Pipeline* pipeline)
{
	m_dx = dx;
    m_uploader = uploader;
    m_srvHeap = heap;
    m_pipeline = pipeline;

    m_splashTex = new Texture2D();
    if (m_splashTex->LoadFromFileWIC(dx->GetDevice(), uploader, heap, L"../../res/SplashScreen.png", true) == false)
        return false;

    // IMPORTANT: le SRV est déjà alloué via srvHeap->Allocate() dans Texture2D. 
    m_splashSrv = m_splashTex->GetSrv();

    // 2) VB pour quad fullscreen
    if (CreateFullscreenQuadVB(dx->GetDevice()) == false) return false;
	if (CreateTintConstantBuffer(dx->GetDevice()) == false) return false;

	return true;
}

void UIRender::Render(ID3D12GraphicsCommandList* cmd, const UIFrame& ui)
{
    if (cmd == nullptr || ui.showSplash == false) return;

    DrawSplash(cmd);
}

void UIRender::UpdateTint(float r, float g, float b, float a)
{
    uint8_t* dst = m_uiCBMapped;

    float* f = reinterpret_cast<float*>(dst);
    f[0] = r; f[1] = g; f[2] = b; f[3] = a;
}

void UIRender::Shutdown()
{
    SafeRelease(m_quadVB);
    delete m_splashTex;
}

bool UIRender::CreateFullscreenQuadVB(ID3D12Device* device)
{
    const UIVertex verts[6] =
    {
        // tri 1
        { {-1.f, -1.f}, {+0.f, +1.f}, 0xFFFFFFFF},
        { {-1.f, +1.f}, {+0.f, +0.f}, 0xFFFFFFFF},
        { {+1.f, +1.f}, {+1.f, +0.f}, 0xFFFFFFFF},

        // tri 2
        { {-1.f, -1.f}, {+0.f, +1.f}, 0xFFFFFFFF},
        { {+1.f, +1.f}, {+1.f, +0.f}, 0xFFFFFFFF},
        { {+1.f, -1.f}, {+1.f, +1.f}, 0xFFFFFFFF},
    };

    const UINT vbSize = (UINT)sizeof(verts);

    auto heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    auto bufDesc = CD3DX12_RESOURCE_DESC::Buffer(vbSize);

    HRESULT hr = device->CreateCommittedResource(
        &heapProps,
        D3D12_HEAP_FLAG_NONE,
        &bufDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&m_quadVB)
    );

    if (FAILED(hr)) return false;

    void* mapped = nullptr;
    CD3DX12_RANGE range(0, 0);
    hr = m_quadVB->Map(0, &range, &mapped);

    if (FAILED(hr)) return false;

    memcpy(mapped, verts, vbSize);
    m_quadVB->Unmap(0, nullptr);

    m_quadVBV.BufferLocation = m_quadVB->GetGPUVirtualAddress();
    m_quadVBV.SizeInBytes = vbSize;
    m_quadVBV.StrideInBytes = sizeof(UIVertex);

    return true;
}

bool UIRender::CreateTintConstantBuffer(ID3D12Device* device)
{
    if (device == nullptr)
        return false;

    auto heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    auto bufDesc = CD3DX12_RESOURCE_DESC::Buffer(256);

    HRESULT hr = device->CreateCommittedResource(
        &heapProps,
        D3D12_HEAP_FLAG_NONE,
        &bufDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&m_uiCB)
    );

    if (FAILED(hr)) return false;

    void* mapped = nullptr;
    CD3DX12_RANGE range(0, 0);
    hr = m_uiCB->Map(0, &range, &mapped);

    if (FAILED(hr)) return false;

    m_uiCBMapped = reinterpret_cast<uint8_t*>(mapped);

    return true;
}

void UIRender::DrawSplash(ID3D12GraphicsCommandList* cmd)
{
    // Bind PSO/RS UI
    cmd->SetPipelineState(m_pipeline->GetUIPSO());
    cmd->SetGraphicsRootSignature(m_pipeline->GetUIRootSignature());

    ID3D12DescriptorHeap* heaps[] = { m_srvHeap->GetHeap() };
    cmd->SetDescriptorHeaps(1, heaps);

    UpdateTint( 1.f, 1.f, 1.f, 1.f);

    // Root param 0 = CBV(b0)
    cmd->SetGraphicsRootConstantBufferView(0, GetTintCBAddress());
    
    // Root param 1 = SRV table t0 (comme ton root signature)
    cmd->SetGraphicsRootDescriptorTable(1, m_splashSrv.gpu);

    cmd->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    cmd->IASetVertexBuffers(0, 1, &m_quadVBV);
    cmd->DrawInstanced(6, 1, 0, 0);
}

#endif // !UI_RENDERER_CPP_INCLUDED