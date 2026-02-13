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
	std::vector<Entity*> m_entities;

	Entity* m_cube;
	Entity* m_sphere;
	Entity* m_cylinder;
	Entity* m_moon;

	Entity* m_rail;
	std::vector<Entity*> m_rails;
	Entity* m_cam;

	float m_speedPlayer = 0.001f;


	//TEST ENTITIES
	Entity* CreateSphere();
	Entity* CreateCylinder();
	Entity* CreateCube();
	Entity* CreateMoon();

	//RAILS
	void CreateRail();
	void ProceduralRails();

	//LOOK WITH MOUSE
	void MoveCamera();
};

