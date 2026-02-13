#include "StateMachineComponent.h"

void StateMachineComponent::SetStateMachine(Entity* entity)
{
	m_stateMachine = new StateMachine();
	m_stateMachine->SetOwner(entity);
}
