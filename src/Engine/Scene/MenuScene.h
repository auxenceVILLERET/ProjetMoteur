#pragma once
#include "Engine/Scene/Scene.h"
#include <vector>	

class Entity;

class MenuScene : public Scene
{
public:
	~MenuScene() = default;
	void Initialize(ECS* ecs, Renderer* renderer, Engine* engine) override;
	void OnEnter() override;
	void OnExit() override;
	void Update(float dt) override;

private:

	std::vector<Entity*> m_entities;
};

