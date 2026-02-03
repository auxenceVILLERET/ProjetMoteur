#include "Entity.h"
#include "ECS.h"

template<typename T>
T* ECS::CreateEntity()
{
	T* entity = new T();
	Entity* castedEntity = dynamic_cast<Entity*>(entity);
	if (castedEntity == nullptr)
	{
		delete entity;
		return nullptr;
	}
	castedEntity->SetId(m_Id++);

	m_Entities.push_back(castedEntity);
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

template<typename T>
T* ECS::AddComponent(Entity* owner)
{
	T* component = new T();
	Component* castedComponent = dynamic_cast<Component*>(component);
	if (castedComponent == nullptr)
	{
		delete component;
		return nullptr;
	}
	castedComponent->SetEntity(owner);
	m_Components.push_back(castedComponent);
	return component;
}

template<typename T>
inline T* ECS::GetComponent(Entity* owner)
{
	for (Component* component : m_Components)
	{
		T* castedComponent = dynamic_cast<T*>(component);
		if (castedComponent != nullptr)
		{
			Entity* entity = nullptr;
			entity = castedComponent->GetEntity();
			if (entity == owner)
			{
				return castedComponent;
			}
		}
	}
	return nullptr;
}
