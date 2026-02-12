#ifndef PIPELINE_CPP_INCLUDED
#define PIPELINE_CPP_INCLUDED

#include "Pipeline.h"

static UINT ShaderCompileFlags()
{
#if defined(DEBUG) || defined(_DEBUG)
    return D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
    return D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif
}

bool Pipeline::InitializeGraphics(ID3D12Device* device, const std::wstring& vsFile, const std::string& vsEntry, const std::wstring& psFile, const std::string& psEntry, const std::vector<D3D12_INPUT_ELEMENT_DESC>& inputLayout, DXGI_FORMAT rtvFormat, DXGI_FORMAT dsvFormat, bool enableDepth)
{
    if (device == nullptr) return false;

    Shutdown();

    // 1) Root Signature (version ultra simple : aucun paramètre root)
    if (BuildRootSignature(device) == false)
        return false;

    // 2) Compile shaders
    if (CompileShader(vsFile, vsEntry, "vs_5_0", m_vs) == false) return false;
    if (CompileShader(psFile, psEntry, "ps_5_0", m_ps) == false) return false;

    // 3) Remplir le PSO
    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
    psoDesc.pRootSignature = m_rootSig;

    psoDesc.VS = { m_vs->GetBufferPointer(), m_vs->GetBufferSize() };
    psoDesc.PS = { m_ps->GetBufferPointer(), m_ps->GetBufferSize() };

    psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    psoDesc.SampleMask = UINT_MAX;
    psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    psoDesc.DepthStencilState = enableDepth
        ? CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT)
        : CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

    if (enableDepth == false)
    {
        psoDesc.DepthStencilState.DepthEnable = FALSE;
        psoDesc.DepthStencilState.StencilEnable = FALSE;
    }

    psoDesc.InputLayout = { inputLayout.data(), (UINT)inputLayout.size() };
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

    psoDesc.NumRenderTargets = 1;
    psoDesc.RTVFormats[0] = rtvFormat;

    psoDesc.DSVFormat = enableDepth ? dsvFormat : DXGI_FORMAT_UNKNOWN;

    psoDesc.SampleDesc.Count = 1;
    psoDesc.SampleDesc.Quality = 0;

    HRESULT hr = device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pso));
    if (FAILED(hr))
        return false;

    return true;
}

void Pipeline::Shutdown()
{
    SafeRelease(m_pso);
    SafeRelease(m_rootSig);
    SafeRelease(m_vs);
    SafeRelease(m_ps);
}

bool Pipeline::BuildRootSignature(ID3D12Device* device)
{
    // 1 paramètre root: CBV b0 (visible vertex shader)
    D3D12_ROOT_PARAMETER param = {};
    param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
    param.Descriptor.ShaderRegister = 0; // b0
    param.Descriptor.RegisterSpace = 0;
    param.ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;

    D3D12_ROOT_SIGNATURE_DESC desc = {};
    desc.NumParameters = 1;
    desc.pParameters = &param;
    desc.NumStaticSamplers = 0;
    desc.pStaticSamplers = nullptr;
    desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    ID3DBlob* serialized = nullptr;
    ID3DBlob* error = nullptr;

    HRESULT hr = D3D12SerializeRootSignature(
        &desc,
        D3D_ROOT_SIGNATURE_VERSION_1,
        &serialized,
        &error
    );

    if (FAILED(hr))
    {
        if (error) OutputDebugStringA((char*)error->GetBufferPointer());
        SafeRelease(error);
        SafeRelease(serialized);
        return false;
    }

    hr = device->CreateRootSignature(
        0,
        serialized->GetBufferPointer(),
        serialized->GetBufferSize(),
        IID_PPV_ARGS(&m_rootSig)
    );

    SafeRelease(error);
    SafeRelease(serialized);
    return SUCCEEDED(hr);
}

bool Pipeline::CompileShader(const std::wstring& file, const std::string& entry, const std::string& target,
    ID3DBlob*& outBlob)
{
    SafeRelease(outBlob);

    ID3DBlob* errors = nullptr;

    HRESULT hr = D3DCompileFromFile(
        file.c_str(),
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        entry.c_str(),
        target.c_str(),
        ShaderCompileFlags(),
        0,
        &outBlob,
        &errors
    );

    if (FAILED(hr))
    {
        if (errors)
        {
            OutputDebugStringA((const char*)errors->GetBufferPointer());
        }
        SafeRelease(errors);
        return false;
    }

    SafeRelease(errors);
    return SUCCEEDED(hr);
}

#endif // !PIPELINE_CPP_INCLUDED