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

void Mesh::Cube()
{
	std::array<Vertex, 8> verticesCube =
	{
		Vertex({ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::DarkCyan) }),
		Vertex({ XMFLOAT3(-1.0f, +1.0f, -1.0f), XMFLOAT4(Colors::DarkCyan) }),
		Vertex({ XMFLOAT3(+1.0f, +1.0f, -1.0f), XMFLOAT4(Colors::DarkCyan) }),
		Vertex({ XMFLOAT3(+1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::DarkCyan) }),
		Vertex({ XMFLOAT3(-1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::DarkCyan) }),
		Vertex({ XMFLOAT3(-1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::DarkCyan) }),
		Vertex({ XMFLOAT3(+1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::DarkCyan) }),
		Vertex({ XMFLOAT3(+1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::DarkCyan) })
	};

	std::array<std::uint16_t, 36> indicesCube =
	{
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7
	};

	m_indexCount = (UINT)indicesCube.size();
}

#endif // !MESH_CPP_INCLUDED


