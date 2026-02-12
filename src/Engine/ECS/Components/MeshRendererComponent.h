#pragma once
#include "Engine/ECS/Component.h"
#include "Render/Helpers/d3dUtil.h" 

class Mesh;
class Renderer;

using namespace DirectX;

class MeshRendererComponent : public Component
{
public:
	void SetMesh(Mesh* newMesh, Renderer* renderer);
	Mesh* GetMesh() const { return m_mesh; }

	bool CreateConstantBuffer(ID3D12Device* device);
	void UpdateConstants(const XMFLOAT4X4& worldViewProjT); // matrice déjà transposée
	D3D12_GPU_VIRTUAL_ADDRESS GetCbAddress() const { return m_pCb ? m_pCb->GetGPUVirtualAddress() : 0; }

private:
	Mesh* m_mesh = nullptr;

	ID3D12Resource* m_pCb = nullptr;
	UINT8* m_pCbMapped = nullptr;
	UINT m_cbSizeAligned = 0;

};

