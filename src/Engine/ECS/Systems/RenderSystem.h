#pragma once
#include "Engine/ECS/System.h"

class Renderer;

class RenderSystem : public System
{
public:
	virtual void Update(ECS& ecs, float deltaTime) override;

private:
	Renderer* m_renderer = nullptr;

};

