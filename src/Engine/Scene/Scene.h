#pragma once
class Scene
{
public:
	virtual ~Scene() = default;

	virtual void OnEnter() = 0;
	virtual void OnExit() = 0;

	virtual void Update(float dt) = 0;
};

