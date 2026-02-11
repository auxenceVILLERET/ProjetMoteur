#ifndef MESH_CPP_INCLUDED
#define MESH_CPP_INCLUDED

#include "Mesh.h"

#include "UploadContext.h"

bool Mesh::Initialize(UploadContext& uploader, const Vertex* vertices, uint32_t vertexCount, const uint16_t* indices, uint32_t indexCount)
{
	if (vertices == NULL || vertexCount == 0 || indices == NULL|| indexCount == 0)
		return false;

	Release();
	m_indexCount = indexCount;

	const uint64_t vbBytes = uint64_t(vertexCount) * sizeof(Vertex);
	const uint64_t ibBytes = uint64_t(indexCount) * sizeof(uint16_t);

	if (uploader.UploadBuffer(vertices, vbBytes, m_pVb, m_pVbUpload, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER) == false)
		return false;

	m_vbView.BufferLocation = m_pVb->GetGPUVirtualAddress();
	m_vbView.SizeInBytes = (UINT)vbBytes;
	m_vbView.StrideInBytes = sizeof(Vertex);

	if (uploader.UploadBuffer(indices, ibBytes, m_pIb, m_pIbUpload, D3D12_RESOURCE_STATE_INDEX_BUFFER) == false)
		return false;

	m_ibView.BufferLocation = m_pIb->GetGPUVirtualAddress();
	m_ibView.SizeInBytes = (UINT)ibBytes;
	m_ibView.Format = DXGI_FORMAT_R16_UINT;

	return true;
}

bool Mesh::CreateTriangle(UploadContext& uploader)
{
	Vertex vertices[] =
	{
		{ { 0.0f,  0.5f, 0.0f }, { 1,0,0,1 } },
		{ { 0.5f, -0.5f, 0.0f }, { 0,1,0,1 } },
		{ { -0.5f,-0.5f, 0.0f }, { 0,0,1,1 } },
	};
	uint16_t indices[] = {0,1,2};

	return Initialize(uploader, vertices, _countof(vertices), indices, _countof(indices));
}

bool Mesh::CreateQuad(UploadContext& uploader)
{
	Vertex vertices[] =
	{
		{ { -0.5f,  0.5f, 0.0f }, { 1,1,0,1 } },
		{ {  0.5f,  0.5f, 0.0f }, { 0,1,1,1 } },
		{ {  0.5f, -0.5f, 0.0f }, { 1,0,1,1 } },
		{ { -0.5f, -0.5f, 0.0f }, { 1,1,1,1 } },
	};
	uint16_t indices[] = {0,1,2, 0,2,3};
	return Initialize(uploader, vertices, _countof(vertices), indices, _countof(indices));
}

bool Mesh::CreateCube(UploadContext& uploader)
{
	Vertex vertices[] =
	{
		{ { -1.0f, -1.0f, -1.0f }, { 1,0,0,1 } },
		{ { -1.0f, +1.0f, -1.0f }, { 0,1,0,1 } },
		{ { +1.0f, +1.0f, -1.0f }, { 0,0,1,1 } },
		{ { +1.0f, -1.0f, -1.0f }, { 1,1,0,1 } },
		{ { -1.0f, -1.0f, +1.0f }, { 1,0,1,1 } },
		{ { -1.0f, +1.0f, +1.0f }, { 0,1,1,1 } },
		{ { +1.0f, +1.0f, +1.0f }, { 1,1,1,1 } },
		{ { +1.0f, -1.0f, +1.0f }, { 0,0,0,1 } }
	};
	uint16_t indices[] =
	{
		// front face
		0, 1, 2,
		0, 2, 3,
		// back face
		4,6,5,
		4,7,6,
		// left face
		4,5,1,
		4,1,0,
		// right face
		3,2,6,
		3,6,7,
		// top face
		1,5,6,
		1,6,2,
		// bottom face
		4,0,3,
		4,3,7
	};
	return Initialize(uploader, vertices, _countof(vertices), indices, _countof(indices));
}

bool Mesh::CreateCylinder(UploadContext& uploader)
{
	Vertex vertices[] =
	{
		// Base (y = 0)
	   Vertex({ XMFLOAT3(1.0000f, 0.0f,  0.0000f) }),
	   Vertex({ XMFLOAT3(0.9239f, 0.0f,  0.3827f) }),
	   Vertex({ XMFLOAT3(0.7071f, 0.0f,  0.7071f) }),
	   Vertex({ XMFLOAT3(0.3827f, 0.0f,  0.9239f) }),
	   Vertex({ XMFLOAT3(0.0000f, 0.0f,  1.0000f) }),
	   Vertex({ XMFLOAT3(-0.3827f, 0.0f,  0.9239f) }),
	   Vertex({ XMFLOAT3(-0.7071f, 0.0f,  0.7071f) }),
	   Vertex({ XMFLOAT3(-0.9239f, 0.0f,  0.3827f) }),
	   Vertex({ XMFLOAT3(-1.0000f, 0.0f,  0.0000f) }),
	   Vertex({ XMFLOAT3(-0.9239f, 0.0f, -0.3827f) }),
	   Vertex({ XMFLOAT3(-0.7071f, 0.0f, -0.7071f) }),
	   Vertex({ XMFLOAT3(-0.3827f, 0.0f, -0.9239f) }),
	   Vertex({ XMFLOAT3(0.0000f, 0.0f, -1.0000f) }),
	   Vertex({ XMFLOAT3(0.3827f, 0.0f, -0.9239f) }),
	   Vertex({ XMFLOAT3(0.7071f, 0.0f, -0.7071f) }),
	   Vertex({ XMFLOAT3(0.9239f, 0.0f, -0.3827f) }),

	   // Haut (y = 1)
	   Vertex({ XMFLOAT3(1.0000f, 1.0f,  0.0000f) }),
	   Vertex({ XMFLOAT3(0.9239f, 1.0f,  0.3827f) }),
	   Vertex({ XMFLOAT3(0.7071f, 1.0f,  0.7071f) }),
	   Vertex({ XMFLOAT3(0.3827f, 1.0f,  0.9239f) }),
	   Vertex({ XMFLOAT3(0.0000f, 1.0f,  1.0000f) }),
	   Vertex({ XMFLOAT3(-0.3827f, 1.0f,  0.9239f) }),
	   Vertex({ XMFLOAT3(-0.7071f, 1.0f,  0.7071f) }),
	   Vertex({ XMFLOAT3(-0.9239f, 1.0f,  0.3827f) }),
	   Vertex({ XMFLOAT3(-1.0000f, 1.0f,  0.0000f) }),
	   Vertex({ XMFLOAT3(-0.9239f, 1.0f, -0.3827f) }),
	   Vertex({ XMFLOAT3(-0.7071f, 1.0f, -0.7071f) }),
	   Vertex({ XMFLOAT3(-0.3827f, 1.0f, -0.9239f) }),
	   Vertex({ XMFLOAT3(0.0000f, 1.0f, -1.0000f) }),
	   Vertex({ XMFLOAT3(0.3827f, 1.0f, -0.9239f) }),
	   Vertex({ XMFLOAT3(0.7071f, 1.0f, -0.7071f) }),
	   Vertex({ XMFLOAT3(0.9239f, 1.0f, -0.3827f) }),

	   // top and bottom middle point
	   Vertex({ XMFLOAT3(0.0f, 0.0f, 0.0f) }),
	   Vertex({ XMFLOAT3(0.0f, 1.0f, 0.0f) }),
	};

	for (size_t i = 0; i < _countof(vertices); i++)
	{
		vertices[i].Color.x = ((i * (1.0f / _countof(vertices))));
	}

	uint16_t indices[] =
	{
		// DRAW TRIANGLES HERE
		0, 16, 1,
		1, 16, 17,

		1, 17, 2,
		2, 17, 18,

		2, 18, 3,
		3, 18, 19,

		3, 19, 4,
		4, 19, 20,

		4, 20, 5,
		5, 20, 21,

		5, 21, 6,
		6, 21, 22,

		6, 22, 7,
		7, 22, 23,

		7, 23, 8,
		8, 23, 24,

		8, 24, 9,
		9, 24, 25,

		9, 25, 10,
		10, 25, 26,

		10, 26, 11,
		11, 26, 27,

		11, 27, 12,
		12, 27, 28,

		12, 28, 13,
		13, 28, 29,

		13, 29, 14,
		14, 29, 30,

		14, 30, 15,
		15, 30, 31,

		// fermeture
		15, 31, 0,
		0, 31, 16,

		// Cap bas (y = 0)
		32, 0, 1,
		32,  1,2,
		32,  2,3,
		32,  3,4,
		32,  4,5,
		32, 5, 6,
		32, 6, 7,
		32, 7, 8,
		32,  8,9,
		32,  9,10,
		32,  10,11,
		32,  11,12,
		32,  12,13,
		32,  13,14,
		32, 14,15,
		32,15, 0,

		// Cap haut (y = 1)
		33, 17, 16,
		33, 18, 17,
		33, 19, 18,
		33, 20, 19,
		33, 21, 20,
		33, 22, 21,
		33, 23, 22,
		33, 24, 23,
		33, 25, 24,
		33, 26, 25,
		33, 27, 26,
		33, 28, 27,
		33, 29, 28,
		33, 30, 29,
		33, 31, 30,
		33, 16, 31,
	};

	return Initialize(uploader, vertices, _countof(vertices), indices, _countof(indices));
}

bool Mesh::CreateIco(UploadContext& uploader)
{
	float a = 0.525731f; // 2PI/12
	float b = 0.850651f;

	Vertex vertices[] =
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

	for (size_t i = 0; i < _countof(vertices); i++)
	{
		vertices[i].Color.x = ((i * (1.0f / _countof(vertices))));
	}

	uint16_t indices[] =
	{
		// DRAW TRIANGLES HERE

		1,4,0,  4,9,0,  4,5,9,  8,5,4,  1,8,4,
		1,10,8, 10,3,8, 8,3,5,  3,2,5,  3,7,2,
		3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0,
		10,1,6, 11,0,9, 2,11,9, 5,2,9,  11,2,7
	};

	return Initialize(uploader, vertices, _countof(vertices), indices, _countof(indices));
}

bool Mesh::CreateConstantBuffer(ID3D12Device* device)
{
	if (device == nullptr) return false;

	// align 256 bytes obligatoire pour CBV en D3D12
	m_cbSizeAligned = (sizeof(ObjectConstants) + 255) & ~255u;

	D3D12_HEAP_PROPERTIES heapProps = {};
	heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;

	D3D12_RESOURCE_DESC desc = {};
	desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Width = m_cbSizeAligned;
	desc.Height = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.SampleDesc.Count = 1;
	desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	HRESULT hr = device->CreateCommittedResource(
		&heapProps,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_pCb));

	if (FAILED(hr) || m_pCb == nullptr)
		return false;

	hr = m_pCb->Map(0, nullptr, reinterpret_cast<void**>(&m_pCbMapped));
	if (FAILED(hr) || m_pCbMapped == nullptr)
		return false;

	// init à identité
	ObjectConstants oc{};
	DirectX::XMStoreFloat4x4(&oc.WorldViewProj, DirectX::XMMatrixIdentity());
	memcpy(m_pCbMapped, &oc, sizeof(oc));

	return true;
}

void Mesh::UpdateConstants(const DirectX::XMFLOAT4X4& worldViewProjT)
{
	if (m_pCbMapped == nullptr) return;

	ObjectConstants oc{};
	oc.WorldViewProj = worldViewProjT;
	memcpy(m_pCbMapped, &oc, sizeof(oc));
}

void Mesh::Release()
{
	SafeRelease(m_pVb);
	SafeRelease(m_pVbUpload);
	SafeRelease(m_pIb);
	SafeRelease(m_pIbUpload);
	m_indexCount = 0;
	SafeRelease(m_pCb);
	m_pCbMapped = nullptr;
	m_cbSizeAligned = 0;
}

void Mesh::FinalizeUpload()
{
	SafeRelease(m_pVbUpload);
	SafeRelease(m_pIbUpload);
}

void Mesh::Draw(ID3D12GraphicsCommandList* cmdList) const
{
	cmdList->IASetVertexBuffers(0, 1, &m_vbView);
	cmdList->IASetIndexBuffer(&m_ibView);
	cmdList->DrawIndexedInstanced(m_indexCount, 1, 0, 0, 0);
}

#endif // !MESH_CPP_INCLUDED


