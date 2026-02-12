#pragma once
#include "DirectXMath.h"
#include <d3d12.h>

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

	void Draw(ID3D12GraphicsCommandList* cmdList) const;

	uint32_t IndexCount() const { return m_indexCount; }

	void Release();

private:
	ID3D12Resource* m_pVb = nullptr;
	ID3D12Resource* m_pVbUpload = nullptr;
	D3D12_VERTEX_BUFFER_VIEW m_vbView{};

	ID3D12Resource* m_pIb = nullptr;
	ID3D12Resource* m_pIbUpload = nullptr;
	D3D12_INDEX_BUFFER_VIEW m_ibView{};

	uint32_t m_indexCount = 0;
};