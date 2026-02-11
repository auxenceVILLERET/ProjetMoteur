#include <d3d12.h> 
#include "RenderSystem.h"
#include "Engine/ECS/ECS.h"
#include "Engine/ECS/Entity.h"
#include "Engine/ECS/Components/MeshRendererComponent.h"
#include "Renderer.h"

void RenderSystem::Update(ECS& ecs, float deltaTime)
{
	ID3D12GraphicsCommandList* cmdList = m_renderer->GetDxContext()->GetCommandList();
}