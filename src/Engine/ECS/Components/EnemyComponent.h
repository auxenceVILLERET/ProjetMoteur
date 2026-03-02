#pragma once
#include "Engine/ECS/Component.h"
class EnemyComponent : public Component
{
public:

	void SetID(int id) { m_id = id; }
	int GetID() const { return m_id; }

private:
	int m_id = 0;
};

