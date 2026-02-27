#ifndef RENDER_RESOURCE_MANAGER_CPP_INCLUDED
#define RENDER_RESOURCE_MANAGER_CPP_INCLUDED

#include "RenderResourceManager.h"

#include "Render/Mesh.h"
#include "Render/DxContext.h"
#include "Render/UploadContext.h"
#include "Render/DescriptorHeapManager.h"
#include "Render/Texture2D.h"

static RenderResourceManager* s_instance = nullptr;

RenderResourceManager& RenderResourceManager::Instance()
{
	if (s_instance == nullptr)
		s_instance = new RenderResourceManager();
	return *s_instance;
}

void RenderResourceManager::Initialize(DxContext* dx, UploadContext* uploader, DescriptorHeapManager* srvHeap)
{
	m_dx = dx;
	m_uploader = uploader;
	m_srvHeap = srvHeap;
}

Mesh* RenderResourceManager::ResolveMesh(MeshHandle h)
{
    if (!h || m_uploader == nullptr) return nullptr;

    auto it = m_meshes.find(h.id);
    if (it != m_meshes.end())
        return it->second.get();

    MeshPreset bm = static_cast<MeshPreset>(h.id);
    Mesh* created = CreateMeshPreset(bm);
    if (created == nullptr) 
        return nullptr;

    m_meshes.emplace(h.id, std::unique_ptr<Mesh>(created));
    return created;
}

Texture2D* RenderResourceManager::ResolveTexture(TextureHandle h)
{
    if (!h || !m_dx || !m_uploader || !m_srvHeap) return nullptr;

    auto it = m_textures.find(h.id);
    if (it != m_textures.end())
        return it->second.get();

    const TextureDesc* desc = ResourceManager::Instance().GetTextureDesc(h);
    if (desc == nullptr) return nullptr;

    Texture2D* created = CreateTextureFromDesc(*desc);
    if (created == nullptr) return nullptr;

    m_textures.emplace(h.id, std::unique_ptr<Texture2D>(created));
    return created;
}

Mesh* RenderResourceManager::CreateMeshPreset(MeshPreset mesh)
{
    auto m = std::make_unique<Mesh>();
    bool ok = false;

    m_uploader->Begin();

    switch (mesh)
    {
    case MeshPreset::Cube:
        ok = m->CreateCube(*m_uploader); 
        break;
    case MeshPreset::Cylinder:
        ok = m->CreateCylinder(*m_uploader); 
        break;
    case MeshPreset::Sphere: 
        ok = m->CreateSphere(*m_uploader);
        break;
    default: 
        ok = false;
        break;
    }

	m_uploader->EndAndWait();

    if (ok == false)
        return nullptr;

    return m.release();
}

Texture2D* RenderResourceManager::CreateTextureFromDesc(const TextureDesc& desc)
{
    auto t = std::make_unique<Texture2D>();
    ID3D12Device* device = m_dx->GetDevice();
    if (device == nullptr)
        return nullptr;

    if (t->LoadFromFileWIC(device, m_uploader, m_srvHeap, desc.path.c_str(), desc.srgb) == false)
        return nullptr;

    return t.release();
}

void RenderResourceManager::FinalizeUpload()
{
    // Free per-resource upload buffers for meshes.
    for (auto& mesh : m_meshes)
        mesh.second->FinalizeUpload();
}

void RenderResourceManager::ReleaseAll()
{
    m_textures.clear();
    m_meshes.clear();
}

#endif // !RENDER_RESSOURCE_MANAGER_CPP_INCLUDED