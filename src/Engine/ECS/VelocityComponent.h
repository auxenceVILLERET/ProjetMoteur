#pragma once
#include "Engine/ECS/Component.h"

struct VelocityComponent : public Component
{
	VelocityComponent() { m_TypeId = 2; }

	float vx = 0.0f;
	float vy = 0.0f;
	float vz = 0.0f;
};

