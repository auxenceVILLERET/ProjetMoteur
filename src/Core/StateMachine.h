#pragma once

class State;
class Entity;

class StateMachine
{
public:
	void ChangeState(State* newState);
	void Update(float dt);
	State* GetCurrentState() const { return currentState; }

	void SetOwner(Entity* entity) { owner = entity; }
	Entity* GetOwner() const { return owner; }

private:
	Entity* owner = nullptr;
	State* currentState = nullptr;
};

