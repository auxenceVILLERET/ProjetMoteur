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

	void Ico() 
	{
		// Spheroid
		//icosahedron (20 faces)

		// normalized
		float a = 0.525731f; // 2PI/12
		float b = 0.850651f;

		//numbers of faces
		static const int n = 12;

		std::array<Vertex, n > vertices =
		{
			Vertex({ XMFLOAT3(-a, 0.0f, +b) }),
			Vertex({ XMFLOAT3(+a, 0.0f, +b) }),
			Vertex({ XMFLOAT3(-a, 0.0f, -b) }),
			Vertex({ XMFLOAT3(+a, 0.0f, -b) }),

			Vertex({ XMFLOAT3(0.0f, +b, +a) }),
			Vertex({ XMFLOAT3(0.0f, +b, -a) }),
			Vertex({ XMFLOAT3(0.0f, -b, +a) }),
			Vertex({ XMFLOAT3(0.0f, -b, -a) }),

			Vertex({ XMFLOAT3(+b,  +a, 0.0f) }),
			Vertex({ XMFLOAT3(-b,  +a, 0.0f) }),
			Vertex({ XMFLOAT3(+b,  -a, 0.0f) }),
			Vertex({ XMFLOAT3(-b,  -a, 0.0f) }),
		};

		for (size_t i = 0; i < n; i++)
		{
			vertices[i].Color.x = ((i * (1.0f / n)));
		}

		std::array<std::uint16_t, 60> indices =
		{
			// DRAW TRIANGLES HERE

			1,4,0,  4,9,0,  4,5,9,  8,5,4,  1,8,4,
			1,10,8, 10,3,8, 8,3,5,  3,2,5,  3,7,2,
			3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0,
			10,1,6, 11,0,9, 2,11,9, 5,2,9,  11,2,7
		};
	}

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