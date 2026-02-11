#pragma once
#include "Engine/ECS/Component.h"
#include "Engine/Mesh.h"

class MeshRendererComponent : public Component
{
public:
	void SetMesh(Mesh* newMesh) { Mesh = newMesh; }
	Mesh* GetMesh() const { return Mesh; }

private:
	Mesh* Mesh = nullptr;
};

