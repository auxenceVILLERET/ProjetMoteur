#include "Engine/ECS/Components/MeshRendererComponent.h"
#include "Engine/Mesh.h"
#include "Render/Renderer.h"
#include "Render/DxContext.h"

void MeshRendererComponent::SetMesh(Mesh* newMesh, Renderer* renderer) 
{ 
	m_mesh = newMesh;
	m_mesh->CreateConstantBuffer(renderer->GetDxContext()->GetDevice());
}