#pragma once
#include <vector>
#include <DirectXMath.h>
#include "Engine/Utils/Shape.h"

using namespace DirectX;

class Entity;
class Renderer;
class ECS;
class Engine;

class Projectile
{
public:
	~Projectile() = default;

	void Initialize(float size, Shape type, int poolSize,float distanceMax,Engine* engine, ECS* ecs, Renderer* renderer);

	Entity* GetAvaibleProjectile();
	void CreatePulling(int poolSize);

	void UpdateDistance();

protected:
	std::vector<Entity*> m_projectiles;

	float m_size;
	Shape m_shape;
	ECS* m_ecs;
	Renderer* m_renderer;
	Engine* m_engine;

	float m_distanceTraveled = 0.0f;
	float m_maxDistance = 50.0f;
};

