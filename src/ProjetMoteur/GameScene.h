#pragma once
#include "Engine/Scene/Scene.h"
#include <vector>


class Entity;
class Engine;
class Camera;
class Projectile;
class Renderer;
class TilesManager;
class Tiles;


class GameScene : public Scene
{
public:
	~GameScene() = default;
	void Initialize(ECS* ecs, Renderer* renderer, Engine* engine, Entity* camera, SceneManager* sceneManager, UIFrame* frame) override;
	void OnEnter() override;
	void OnExit() override;
	void Update(float dt) override;

private:
	float m_deltaTime; // deltaTime
	bool m_startupFlag = true;

	std::vector<Entity*> m_entities = {};

	Entity* m_floor = nullptr;

	Entity* m_rail = nullptr;
	std::vector<Entity*> m_rails = {};

	Entity* m_body = nullptr;
	Entity* m_cam = nullptr;

	float m_pivot = 0.0f;
	int m_currentRailIndex = 0;

	float m_angleOrbit = 0.0f;

	float m_maxRails = 5;

	Entity* m_bullet = nullptr;

	float m_speedPlayer = 1.f;

	float m_yaw = 0.0f;
	float m_pitch = 0.0f;
	bool m_locked = true;

	Projectile* m_projectile = nullptr;
	TilesManager* m_tilesManager = nullptr;
	Tiles* m_tiles = nullptr;
	
	Entity* CreateBody();
	Entity* CreateFloor();


	//RAILS
	void CreateRail();
	void ProceduralRails();
	void DeleteRails();

	void FollowRail();

	//MOVING PLAYER
	void MovePlayer();
	void UpdateCameraTransform();

	//LOOK WITH MOUSE
	void HandleCursor();

	//SHOOTING
	void Shooting();
};

