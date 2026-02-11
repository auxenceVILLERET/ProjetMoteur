#include "RessourceManager.h"
#include "Engine/Mesh.h"

static RessourceManager* s_instance = nullptr;

RessourceManager& RessourceManager::Instance()
{
	if (s_instance == nullptr)
		s_instance = new RessourceManager();
	return *s_instance;
}

void RessourceManager::Initialize(Renderer* renderer, UploadContext* uploader)
{
	m_renderer = renderer;
	m_uploader = uploader;
}

Mesh* RessourceManager::GetCube()
{
	const std::string name = "cube";
	auto it = m_meshes.find(name);
	if (it != m_meshes.end())
		return it->second.get();
	Mesh* mesh = CreateCube();
	m_meshes[name] = std::unique_ptr<Mesh>(mesh);
	return mesh;
}

Mesh* RessourceManager::CreateCube()
{
	Mesh* mesh = new Mesh();
	if (mesh->CreateCube(*m_uploader) == false)
	{
		delete mesh;
		return nullptr;
	}
	return mesh;
}

void RessourceManager::FinalizeUpload()
{
	for (auto& [name, mesh] : m_meshes)
	{
		mesh->FinalizeUpload();
	}
}

