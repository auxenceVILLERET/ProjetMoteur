#pragma once
class Entity;

class Component
{
public:
	Component() = default;
	virtual ~Component() = default;

	int GetTypeId() { return m_TypeId; }
	void SetEntity(Entity* entity) { m_Entity = entity; }
	Entity* GetEntity() { return m_Entity; }

protected:
	int m_TypeId;
	Entity* m_Entity = nullptr;
};

