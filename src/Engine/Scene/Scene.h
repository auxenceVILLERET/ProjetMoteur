#pragma once

class Renderer; 
class ECS;

class Scene
{
public:
	virtual ~Scene() = default;

	virtual void Initialize(ECS* ecs, Renderer* renderer) = 0;
	virtual void OnEnter() = 0;
	virtual void OnExit() = 0;

	virtual void Update(float dt) = 0;


protected:
	Renderer* m_renderer; 
	ECS* m_ecs;
};

