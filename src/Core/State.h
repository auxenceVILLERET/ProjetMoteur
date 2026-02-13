#pragma once
class StateMachine;

class State
{
public:
	virtual ~State() = default;
	virtual void OnEnter(StateMachine& machine) = 0;
	virtual void OnExit(StateMachine& machine) = 0;

	virtual void Update(StateMachine& machine, float dt) = 0;
};

