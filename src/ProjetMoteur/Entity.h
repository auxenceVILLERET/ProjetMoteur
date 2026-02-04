#pragma once
#include <vector>

class Component;

class Entity
{
public:
	Entity() = default;
	virtual ~Entity() = default;

	void SetId(uint32_t id) { m_Id = id; }
	uint32_t GetId() const { return m_Id; }

	template<typename T>
	T* AddComponent();

	template<typename T>
	T* GetComponent();

private:
	uint32_t m_Id = 0;
	std::vector<Component*> m_Components;
};

#include "Entity.inl"
