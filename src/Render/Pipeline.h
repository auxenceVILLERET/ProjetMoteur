#ifndef PIPELINE_H_INCLUDED
#define PIPELINE_H_INCLUDED

#include "Helpers/d3dUtil.h"

class Pipeline
{
public:
    Pipeline() = default;
	~Pipeline() {Shutdown();}

	bool InitializePipeline(ID3D12Device* device);

    bool InitializeGraphics(
        ID3D12Device* device,
        const std::wstring& vsFile, const std::string& vsEntry,
        const std::wstring& psFile, const std::string& psEntry,
        const std::vector<D3D12_INPUT_ELEMENT_DESC>& inputLayout,
        DXGI_FORMAT rtvFormat,
        DXGI_FORMAT dsvFormat,
        bool enableDepth
    );

    void Shutdown();

    ID3D12PipelineState* GetPSO() const { return m_pso; }
    ID3D12RootSignature* GetRootSignature() const { return m_rootSig; }

private:
    bool BuildRootSignature(ID3D12Device* device);
    bool CompileShader(const std::wstring& file, const std::string& entry, const std::string& target,
        ID3DBlob*& outBlob);

private:
    ID3D12RootSignature* m_rootSig;
    ID3D12PipelineState* m_pso;

    // garde pour debug (optionnel)
    ID3DBlob* m_vs;
    ID3DBlob* m_ps;
};

#endif // !PIPELINE_H_INCLUDED