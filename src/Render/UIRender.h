#ifndef UI_RENDER_H_INCLUDED
#define UI_RENDER_H_INCLUDED

#include "Helpers/d3dUtil.h"

using namespace DirectX;

class DxContext;
class UploadContext;
class Texture2D;
class Pipeline;

#include "Render/DescriptorHeapManager.h"

struct UIVertex
{
	XMFLOAT2 Pos;
	XMFLOAT2 UV;
	UINT Color;
};

struct UIConstants
{
	float gTint[4]; // rgba
};

struct UIFrame 
{ 
	bool showSplash = false; 
};

class UIRender
{
public:
	UIRender() = default;
	~UIRender() { Shutdown(); }

	bool Initialize(DxContext* dx, UploadContext* uploader, DescriptorHeapManager* heap, Pipeline* pipeline);
	void Render(ID3D12GraphicsCommandList* cmd, const UIFrame& ui);

	void UpdateTint(float r, float g, float b, float a);

	D3D12_GPU_VIRTUAL_ADDRESS GetTintCBAddress() const { return m_uiCB ? m_uiCB->GetGPUVirtualAddress() : 0; }

	void Shutdown();

private:
	bool CreateFullscreenQuadVB(ID3D12Device* device);
	bool CreateTintConstantBuffer(ID3D12Device* device);
	void DrawSplash(ID3D12GraphicsCommandList* cmd);

	DxContext* m_dx = nullptr;
	UploadContext* m_uploader = nullptr;
	Pipeline* m_pipeline = nullptr;
	DescriptorHeapManager* m_srvHeap = nullptr;

	Texture2D* m_splashTex = nullptr;
	DescriptorHandle m_splashSrv;

	ID3D12Resource* m_uiCB = nullptr;
	uint8_t* m_uiCBMapped = nullptr;

	ID3D12Resource* m_quadVB;
	D3D12_VERTEX_BUFFER_VIEW m_quadVBV{};
};

#endif // !UI_RENDER_H_INCLUDED