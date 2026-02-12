#include <d3d12.h> 
#include "RenderSystem.h"
#include "Engine/ECS/ECS.h"
#include "Engine/ECS/Entity.h"
#include "Engine/ECS/Components/MeshRendererComponent.h"
#include "Renderer.h"
#include "Engine/Mesh.h"

void RenderSystem::Update(ECS& ecs, float deltaTime)
{
	m_vMesh.clear();
	for (Entity* entity : ecs.GetEntities()) 
	{ 
		if (entity == nullptr) continue;
		MeshRendererComponent* meshRenderer = entity->GetComponent<MeshRendererComponent>();
		if (meshRenderer != nullptr) 
		{
			XMMATRIX world = XMMatrixTranslation(entity->GetPosition().x, entity->GetPosition().y, entity->GetPosition().z);

			XMFLOAT4X4 worldViewProj = m_renderer->BuildWorldViewProjMatrix(world); 

			meshRenderer->GetMesh()->UpdateConstants(worldViewProj);

			m_vMesh.push_back(meshRenderer->GetMesh());
		} 
	} 
	if (m_renderer != nullptr)
	{ 
		m_renderer->Render(m_vMesh);
	}
}