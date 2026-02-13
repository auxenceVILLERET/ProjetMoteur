#include "Engine/ECS/Components/MeshRendererComponent.h"
#include "Engine/Mesh.h"
#include "Render/Renderer.h"
#include "Render/DxContext.h"

void MeshRendererComponent::SetMesh(Mesh* newMesh, Renderer* renderer) 
{ 
	m_mesh = newMesh;
	CreateConstantBuffer(renderer->GetDxContext()->GetDevice());
}

bool MeshRendererComponent::CreateConstantBuffer(ID3D12Device* device) 
{ 
	if (device == nullptr || m_mesh == nullptr) return false;

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

void MeshRendererComponent::UpdateConstants(const DirectX::XMFLOAT4X4& worldViewProjT)
{ 
	if (m_pCbMapped == nullptr || m_mesh == nullptr) return;

	ObjectConstants oc{};
	oc.WorldViewProj = worldViewProjT;
	memcpy(m_pCbMapped, &oc, sizeof(oc));
}