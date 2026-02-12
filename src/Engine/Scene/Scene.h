#pragma once

class Renderer; 
class ECS;

class Scene
{
public:
	virtual ~Scene() = default;

	virtual void OnEnter() = 0;
	virtual void OnExit() = 0;

	virtual void Update(float dt) = 0;

	void SetRenderer(Renderer* renderer) { m_renderer = renderer; } 
	void SetECS(ECS* ecs) { m_ecs = ecs; }

protected:
	Renderer* m_renderer; 
	ECS* m_ecs;
};

