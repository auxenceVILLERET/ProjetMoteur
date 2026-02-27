#ifndef MESH_RENDERER_COMPONENT_CPP_INCLUDED
#define MESH_RENDERER_COMPONENT_CPP_INCLUDED

#include "Engine/ECS/Components/MeshRendererComponent.h"
#include "Render/Renderer.h"
#include "Render/Mesh.h"
#include "Render/DxContext.h"
#include "Render/Texture2D.h"

void MeshRendererComponent::SetMesh(MeshHandle h, Renderer* renderer)
{
	m_mesh = h;
	CreateConstantBuffer(renderer->GetDxContext()->GetDevice());
}

bool MeshRendererComponent::CreateConstantBuffer(ID3D12Device* device)
{
	if (device == nullptr)
		return false;

	D3D12_HEAP_PROPERTIES heapProps = {};
	heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;

	D3D12_RESOURCE_DESC desc = {};
	desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Width = sizeof(ObjectConstants);
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

void MeshRendererComponent::UpdateConstants(XMFLOAT4X4& worldViewProjT)
{
	if (m_pCbMapped == nullptr) return;

	ObjectConstants oc{};
	oc.WorldViewProj = worldViewProjT;
	memcpy(m_pCbMapped, &oc, sizeof(oc));
}

#endif // !MESH_RENDERER_COMPONENT_CPP_INCLUDED