#pragma once
#include "Engine/ECS/Component.h"

struct PositionComponent : public Component
{
    PositionComponent() { m_TypeId = 1; }

    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
};

