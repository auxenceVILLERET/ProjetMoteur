#include "StateMachine.h"
#include "State.h"
#include "Engine/ECS/Entity.h"

void StateMachine::ChangeState(State* newState)
{
	if (currentState)
		currentState->OnExit(*this);
	currentState = newState;
	if (currentState)
		currentState->OnEnter(*this);
}

void StateMachine::Update(float dt)
{
	if (currentState)
		currentState->Update(*this, dt);
}

State* StateMachine::GetCurrentState() const
{
	return currentState;
}

void StateMachine::SetOwner(Entity* entity)
{
	owner = entity;
}

Entity* StateMachine::GetOwner() const
{
	return owner;
}
