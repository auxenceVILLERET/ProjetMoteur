#pragma once
#include <vector>

class Entity;
class System;

class ECS
{
public:
	ECS();
	~ECS();

	template<typename T>
	T* CreateEntity();

	template<typename T>
	T* AddSystem();

	std::vector<Entity*>& GetEntities() { return m_Entities; }

	void Update(float deltaTime);

private:
	uint32_t m_Id;
	std::vector<Entity*> m_Entities;
	std::vector<System*> m_Systems;
};

#include "ECS.inl"

