#include <d3d12.h> 
#include "RenderSystem.h"
#include "Engine/ECS/ECS.h"
#include "Engine/ECS/Entity.h"
#include "Engine/ECS/Components/MeshRendererComponent.h"
#include "Renderer.h"
#include "Engine/Mesh.h"

void RenderSystem::Update(ECS& ecs, float deltaTime)
{
	m_vObj.clear();
	for (Entity* entity : ecs.GetEntities()) 
	{ 
		if (entity == nullptr ) continue;
		if (entity->IsActive() == false) continue;

		MeshRendererComponent* meshRenderer = entity->GetComponent<MeshRendererComponent>();
		if (meshRenderer != nullptr) 
		{
			XMFLOAT4X4 m = entity->GetMatrix();
			XMMATRIX world = XMLoadFloat4x4(&m);

			XMFLOAT4X4 worldViewProj = m_renderer->BuildWorldViewProjMatrix(world); 

			meshRenderer->UpdateConstants(worldViewProj);

			m_vObj.push_back(meshRenderer);
		} 
	} 
	if (m_renderer != nullptr)
	{ 
		m_renderer->Render(m_vObj);
	}
}