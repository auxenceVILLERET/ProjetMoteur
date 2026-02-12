#pragma once
#include "Engine/ECS/System.h"
#include <vector>

class Renderer;
class Mesh;

class RenderSystem : public System
{
public:
	virtual void Update(ECS& ecs, float deltaTime) override;
	void SetRenderer(Renderer* renderer) { m_renderer = renderer; }
private:
	Renderer* m_renderer = nullptr;
	std::vector<Mesh*> m_vMesh;
};

