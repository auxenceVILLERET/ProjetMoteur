#pragma once
#include "Engine/ECS/Component.h"

class ObstacleComponent : public Component
{
public:
	void SetKillDistance(float distance) { m_killDistance = distance; }
	float GetKillDistance() const { return m_killDistance; }

	void SetIsActive(bool isActive) { m_isActive = isActive; }
	bool IsActive() const { return m_isActive; }

private:
	float m_killDistance = 2.0f;
	bool m_isActive = true;
};

