#pragma once
#include "Engine/Scene/Scene.h"
#include <vector>

class Entity;


class GameScene : public Scene
{
public:
	~GameScene() = default;
	void Initialize(ECS* ecs, Renderer* renderer) override;
	void OnEnter() override; 
	void OnExit() override;
	void Update(float dt) override;

private:
	std::vector<Entity*> m_entities;
};

