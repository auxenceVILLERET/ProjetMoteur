#include "ECS.h"
#include "System.h"

ECS::ECS()
{
	m_Id = 0;
}

ECS::~ECS()
{
	for (Entity* entity : m_Entities)
	{
		delete entity;
	}
}

void ECS::Update(float deltaTime)
{
	for (System* system : m_Systems)
	{
		system->Update(*this, deltaTime);
	}
}
