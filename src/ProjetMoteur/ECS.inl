#include "Entity.h"
#include "ECS.h"

template<typename T>
T* ECS::CreateEntity()
{
	T* entity = new T();
	entity->SetId(m_Id++);
	m_Entities.push_back(entity);
	return entity;
}

template<typename T>
inline T* ECS::AddSystem()
{
	T* system = new T();
	System* castedSystem = dynamic_cast<System*>(system);
	if (castedSystem == nullptr)
	{
		delete system;
		return nullptr;
	}
	m_Systems.push_back(castedSystem);
	return system;
}

