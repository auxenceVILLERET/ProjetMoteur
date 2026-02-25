#pragma once
#include "Engine/Scene/Scene.h"
#include <vector>


class Entity;
class Engine;
class Camera;

class GameScene : public Scene
{
public:
	~GameScene() = default;
	void Initialize(ECS* ecs, Renderer* renderer, Engine* engine, Entity* camera) override;
	void OnEnter() override; 
	void OnExit() override;
	void Update(float dt) override;

private:
	float deltaTime; // deltaTime

	std::vector<Entity*> m_entities;

	Entity* m_cube;
	Entity* m_sphere;
	Entity* m_cylinder;
	Entity* m_moon;
	Entity* m_floor;

	Entity* m_rail;
	std::vector<Entity*> m_rails;
	Entity* m_cam;

	float m_maxRails = 5;

	Entity* m_bullet;

	float m_speedPlayer = 1.f;


	//TEST ENTITIES
	Entity* CreateSphere();
	Entity* CreateCylinder();
	Entity* CreateCube();
	Entity* CreateMoon();
	Entity* CreateFloor();

	//RAILS
	void CreateRail();
	void ProceduralRails();
	void DeleteRails();

	//LOOK WITH MOUSE
	void MoveCamera();
	void ShootBullet();
};

