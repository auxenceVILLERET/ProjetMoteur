#ifndef MESH_H_INCLUDED
#define MESH_H_INCLUDED

#include "Helpers/d3dUtil.h"

using namespace DirectX;

struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
};

struct ObjectConstants
{
	DirectX::XMFLOAT4X4 WorldViewProj;
};

class UploadContext;

class Mesh
{
public:
	Mesh() = default;
	~Mesh() { Release(); }

	bool Initialize(UploadContext& uploader, const Vertex* vertices, uint32_t vertexCount, const uint16_t* indices, uint32_t indexCount);

	bool CreateTriangle(UploadContext& uploader);
	bool CreateQuad(UploadContext& uploader);
	bool CreateCube(UploadContext& uploader);
	bool CreateCylinder(UploadContext& uploader);
	bool CreateIco(UploadContext& uploader);

	void FinalizeUpload(); // libère les upload buffers (après EndAndWait)
	bool CreateConstantBuffer(ID3D12Device* device);
	void UpdateConstants(const DirectX::XMFLOAT4X4& worldViewProjT); // matrice déjà transposée

	void Draw(ID3D12GraphicsCommandList* cmdList) const;

	uint32_t IndexCount() const { return m_indexCount; }
	D3D12_GPU_VIRTUAL_ADDRESS GetCbAddress() const { return m_pCb ? m_pCb->GetGPUVirtualAddress() : 0; }


	void SetPosition(const XMFLOAT3& pos) { m_position = pos; }
	void SetRotation(const XMFLOAT3& rot) { m_rotation = rot; }
	void SetScale(const XMFLOAT3& scale) { m_scale = scale; }

	void Release();

private:
	ID3D12Resource* m_pVb = nullptr;
	ID3D12Resource* m_pVbUpload = nullptr;
	D3D12_VERTEX_BUFFER_VIEW m_vbView{};

	ID3D12Resource* m_pIb = nullptr;
	ID3D12Resource* m_pIbUpload = nullptr;
	D3D12_INDEX_BUFFER_VIEW m_ibView{};

	uint32_t m_indexCount = 0;

	ID3D12Resource* m_pCb = nullptr;
	UINT8* m_pCbMapped = nullptr;
	UINT m_cbSizeAligned = 0;

	XMFLOAT3 m_position{ 0,0,0 };
	XMFLOAT3 m_rotation{ 0,0,0 }; // radians
	XMFLOAT3 m_scale{ 1,1,1 };
};

#endif // !MESH_H_INCLUDED