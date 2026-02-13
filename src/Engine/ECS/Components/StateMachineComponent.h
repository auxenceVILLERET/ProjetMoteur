#pragma once
#include "Engine/ECS/Component.h"
#include "Core/StateMachine.h"

class StateMachineComponent : public Component
{
public:

	void SetStateMachine(Entity* entity);
	StateMachine* GetStateMachine() const { return m_stateMachine; }
private:
	StateMachine* m_stateMachine;
};

