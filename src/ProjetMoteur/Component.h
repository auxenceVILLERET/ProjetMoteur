#pragma once
class Entity;

class Component
{
public:
	Component() = default;
	virtual ~Component() = default;

	int GetTypeId() { return m_TypeId; }
	void SetEntity(Entity* entity) { m_Entity = entity; }
	
private:
	int m_TypeId = 1;
	Entity* m_Entity = nullptr;
};

