#include "RessourceManager.h"
#include "Engine/Mesh.h"
#include "Render/Renderer.h" 
#include "Render/DxContext.h"

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
	if (m_cubeMesh == nullptr) 
		m_cubeMesh = CreateCube(); 
	return m_cubeMesh;
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
	m_cubeMesh->FinalizeUpload();
}