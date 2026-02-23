#include "Projectile.h"
#include "Engine/ECS/Entity.h"
#include "Engine/RessourceManager.h"
#include "Render/Renderer.h"
#include "Engine/ECS/ECS.h"
#include "Engine/Engine.h"
#include "Engine/ECS/Components/MeshRendererComponent.h"
#include "Engine/ECS/Components/RigidBodyComponent.h"

void Projectile::Initialize(float size, Shape shape, int poolSize,float distanceMax,Engine* engine, ECS* ecs, Renderer* renderer)
{
	m_size = size;
	m_shape = shape;
	m_ecs = ecs;
	m_renderer = renderer;
	m_engine = engine;
	m_maxDistance = distanceMax;

	CreatePulling(poolSize);
}

Entity* Projectile::GetAvaibleProjectile()
{
	for (Entity* projectile : m_projectiles)
	{
		if (!projectile->IsActive())
		{
			projectile->SetActive(true);
			return projectile;
		}
	}
	return nullptr;
}

void Projectile::CreatePulling(int poolSize)
{
	for (int i = 0; i < poolSize; i++)
	{
		Entity* projectile = m_ecs->CreateEntity<Entity>();
		projectile->SetActive(false);
		projectile->AddComponent<MeshRendererComponent>()->SetMesh(RessourceManager::Instance().GetMeshByShape(m_shape), m_renderer);
		projectile->SetScale(m_size);
		projectile->AddComponent<RigidBodyComponent>()->SetUseGravity(false);
		m_projectiles.push_back(projectile);
	}
}

void Projectile::UpdateDistance()
{
	float dt = m_engine->GetDeltaTime();
	for (Entity* projectile : m_projectiles)
	{
		if (projectile->IsActive())
		{
			XMFLOAT3 velocity = projectile->GetComponent<RigidBodyComponent>()->GetVelocity();
			
			m_distanceTraveled += sqrtf(velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z) * dt;
			if (m_distanceTraveled >= m_maxDistance)
			{
				projectile->SetActive(false);
				m_distanceTraveled = 0.0f;
			}
		}
	}
}
