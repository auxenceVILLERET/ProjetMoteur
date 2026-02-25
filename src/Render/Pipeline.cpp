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

bool Pipeline::InitializePipeline(ID3D12Device* device)
{
    // Formats identiques à SwapChainTargets (R8G8B8A8 + D24S8)
    return InitializeGraphics(
        device,
        DXGI_FORMAT_R8G8B8A8_UNORM,
        DXGI_FORMAT_D24_UNORM_S8_UINT,
        true
    );
}

bool Pipeline::InitializeGraphics(ID3D12Device* device, DXGI_FORMAT rtvFormat, DXGI_FORMAT dsvFormat, bool enableDepth)
{
    if (device == nullptr) return false;

    Shutdown();

    if (BuildRootSignature(device) == false) return false;
	if (BuildUIRootSignature(device) == false) return false;

	// --------------------
	// DEFAULT SHADERS
	// --------------------

    if (CompileShader(L"../../res/Default_VS.hlsl", "VSMain", "vs_6_0", m_vs) == false) return false;
    if (CompileShader(L"../../res/Default_PS.hlsl", "PSMain", "ps_6_0", m_ps) == false) return false;

    static D3D12_INPUT_ELEMENT_DESC inputLayout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    };

    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
    psoDesc.pRootSignature = m_defaultRootSig;

    psoDesc.VS = { m_vs->GetBufferPointer(), m_vs->GetBufferSize() };
    psoDesc.PS = { m_ps->GetBufferPointer(), m_ps->GetBufferSize() };

    psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    psoDesc.SampleMask = UINT_MAX;
    psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

    if (enableDepth == false)
    {
        psoDesc.DepthStencilState.DepthEnable = false;
        psoDesc.DepthStencilState.StencilEnable = false;
    }

    psoDesc.InputLayout = { inputLayout, _countof(inputLayout) };
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    psoDesc.NumRenderTargets = 1;
    psoDesc.RTVFormats[0] = rtvFormat;
    psoDesc.DSVFormat = enableDepth ? dsvFormat : DXGI_FORMAT_UNKNOWN;
    psoDesc.SampleDesc.Count = 1;
    psoDesc.SampleDesc.Quality = 0;

    HRESULT hr = device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_defaultPso));
    if (FAILED(hr))
        return false;

    // --------------------
    // UI SHADERS 
    // --------------------
    ID3DBlob* uiVS = nullptr;
    ID3DBlob* uiPS = nullptr;

    // Exemple: fichiers/entry différents pour l'UI
    if (CompileShader(L"../../res/UI_VS.hlsl", "VSMain", "vs_6_0", uiVS) == false) return false;
    if (CompileShader(L"../../res/UI_PS.hlsl", "PSMain", "ps_6_0", uiPS) == false) return false;

    static D3D12_INPUT_ELEMENT_DESC uiLayout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 0,  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 8,  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R8G8B8A8_UNORM,  0, 16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    };

    // PSO UI
    D3D12_GRAPHICS_PIPELINE_STATE_DESC uiDesc = {};
    uiDesc.pRootSignature = m_uiRootSig;

    uiDesc.VS = { uiVS->GetBufferPointer(), uiVS->GetBufferSize() };
    uiDesc.PS = { uiPS->GetBufferPointer(), uiPS->GetBufferSize() };

    uiDesc.SampleMask = UINT_MAX;

    // Rasterizer : pas de cull pour l'UI
    uiDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    uiDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

    // Depth OFF pour overlay
    uiDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
    uiDesc.DepthStencilState.DepthEnable = false;
    uiDesc.DepthStencilState.StencilEnable = false;

    // Blend alpha ON
    uiDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    uiDesc.BlendState.RenderTarget[0].BlendEnable = true;
    uiDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
    uiDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
    uiDesc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
    uiDesc.BlendState.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
    uiDesc.BlendState.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_INV_SRC_ALPHA;
    uiDesc.BlendState.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
    uiDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

    // IA
    uiDesc.InputLayout = { uiLayout, _countof(uiLayout) };
    uiDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

    // RT
    uiDesc.NumRenderTargets = 1;
    uiDesc.RTVFormats[0] = rtvFormat;
    uiDesc.DSVFormat = DXGI_FORMAT_UNKNOWN;

    uiDesc.SampleDesc.Count = 1;
    uiDesc.SampleDesc.Quality = 0;

    HRESULT hr2 = device->CreateGraphicsPipelineState(&uiDesc, IID_PPV_ARGS(&m_uiPso));
    if (FAILED(hr2))
        return false;

    return true;
}

void Pipeline::Shutdown()
{
    SafeRelease(m_defaultPso);
    SafeRelease(m_defaultRootSig);
	SafeRelease(m_uiPso);
	SafeRelease(m_uiRootSig);
    SafeRelease(m_vs);
    SafeRelease(m_ps);
}

bool Pipeline::BuildRootSignature(ID3D12Device* device)
{
    // Param 0 : CBV b0 (VS)
    D3D12_ROOT_PARAMETER params[2] = {};

    params[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
    params[0].Descriptor.ShaderRegister = 0; // b0
    params[0].Descriptor.RegisterSpace = 0;
    params[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;

    // Param 1 : Descriptor table SRV t0 (PS)
    D3D12_DESCRIPTOR_RANGE range = {};
    range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
    range.NumDescriptors = 1;
    range.BaseShaderRegister = 0; // t0
    range.RegisterSpace = 0;
    range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

    params[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
    params[1].DescriptorTable.NumDescriptorRanges = 1;
    params[1].DescriptorTable.pDescriptorRanges = &range;
    params[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

    // Static sampler s0
    D3D12_STATIC_SAMPLER_DESC samp = {};
    samp.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
    samp.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    samp.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    samp.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    samp.ShaderRegister = 0; // s0
    samp.RegisterSpace = 0;
    samp.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
    samp.MinLOD = 0.0f;
    samp.MaxLOD = D3D12_FLOAT32_MAX;
    samp.MaxAnisotropy = 1;
    samp.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;

    D3D12_ROOT_SIGNATURE_DESC desc = {};
    desc.NumParameters = _countof(params);
    desc.pParameters = params;
    desc.NumStaticSamplers = 1;
    desc.pStaticSamplers = &samp;
    desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    ID3DBlob* serialized = nullptr;
    ID3DBlob* error = nullptr;

    HRESULT hr = D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1, &serialized, &error);
    if (FAILED(hr))
    {
        if (error) 
            OutputDebugStringA((char*)error->GetBufferPointer());
        SafeRelease(error);
        SafeRelease(serialized);
        return false;
    }

    hr = device->CreateRootSignature(0, serialized->GetBufferPointer(), serialized->GetBufferSize(), IID_PPV_ARGS(&m_defaultRootSig));
    if (FAILED(hr))
    {
        SafeRelease(error);
        SafeRelease(serialized);
        return false;
	}

    SafeRelease(error);
    SafeRelease(serialized);
    return SUCCEEDED(hr);
}

bool Pipeline::BuildUIRootSignature(ID3D12Device* device)
{
    // Root signature pour UI (param 0 = CBV b0, visible dans tous les shaders)
    D3D12_ROOT_PARAMETER param = {};
    param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
    param.Descriptor.ShaderRegister = 0; // b0
    param.Descriptor.RegisterSpace = 0;
    param.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

    D3D12_ROOT_SIGNATURE_DESC desc = {};
    desc.NumParameters = 1;
    desc.pParameters = &param;
    desc.NumStaticSamplers = 0;
    desc.pStaticSamplers = nullptr;
    desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    ID3DBlob* serialized = nullptr;
    ID3DBlob* error = nullptr;

    HRESULT hr = D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1, &serialized, &error);

    if (FAILED(hr))
    {
        if (error)
            OutputDebugStringA((char*)error->GetBufferPointer());
        SafeRelease(error);
        SafeRelease(serialized);
        return false;
    }

    hr = device->CreateRootSignature(0, serialized->GetBufferPointer(), serialized->GetBufferSize(), IID_PPV_ARGS(&m_uiRootSig));

    if (FAILED(hr))
    {
        SafeRelease(error);
        SafeRelease(serialized);
        return false;
    }

    SafeRelease(error);
    SafeRelease(serialized);

	return SUCCEEDED(hr);
}

bool Pipeline::CompileShader(const std::wstring& file, const std::string& entry, const std::string& target, ID3DBlob*& outBlob)
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