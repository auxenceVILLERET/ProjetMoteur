#include "Component.h"
#include "Entity.h"

template<typename T>
T* Entity::AddComponent()
{
	T* component = new T();
	Component* castedComponent = dynamic_cast<Component*>(component);
	if (castedComponent == nullptr)
	{
		delete component;
		return nullptr;
	}
	castedComponent->SetEntity(this);
	m_Components.push_back(castedComponent);
	return component;
}

template<typename T>
inline T* Entity::GetComponent()
{
	for (Component* component : m_Components)
	{
		T* castedComponent = dynamic_cast<T*>(component);
		if (castedComponent != nullptr)
		{
			return castedComponent;
		}
	}
	return nullptr;
}
