#pragma once
#include "Engine/ECS/Component.h"

class Mesh;
class Renderer;

class MeshRendererComponent : public Component
{
public:
	void SetMesh(Mesh* newMesh, Renderer* renderer);
	Mesh* GetMesh() const { return m_mesh; }

private:
	Mesh* m_mesh = nullptr;
	ID3D12Resource* m_pCb = nullptr;
	UINT8* m_pCbMapped = nullptr;
	UINT m_cbSizeAligned = 0;

};

