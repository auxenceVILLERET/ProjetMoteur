#pragma once
#include "Engine/Scene/Scene.h"
#include <vector>


class Entity;
class Engine;
class Camera;
class Projectile;
class Renderer;


class GameScene : public Scene
{
public:
	~GameScene() = default;
	void Initialize(ECS* ecs, Renderer* renderer, Engine* engine, Entity* camera) override;
	void OnEnter() override;
	void OnExit() override;
	void Update(float dt) override;

private:
	float m_deltaTime; // deltaTime
	bool m_startupFlag = true;

	std::vector<Entity*> m_entities;

	Entity* m_floor;

	Entity* m_rail;
	std::vector<Entity*> m_rails;

	Entity* m_body;
	Entity* m_cam;

	float m_maxRails = 5;

	Entity* m_bullet;

	float m_speedPlayer = 1.f;

	Projectile* m_projectile;

	Entity* CreateBody();
	Entity* CreateFloor();

	//RAILS
	void CreateRail();
	void ProceduralRails();
	void DeleteRails();

	//MOVING PLAYER
	void MovePlayer();

	//LOOK WITH MOUSE
	void MoveCamera();
	void Debug();

	//SHOOTING
	void Shooting();
};

