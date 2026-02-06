#pragma once
class Entity;

class Component
{
    virtual void Start() {}
    virtual void Update(float dt) {}

    Entity* GetEntity() const { return m_Entity; }

    void SetEntity(Entity* e) { m_Entity = e; }

private:
    friend class Entity;

    Entity* m_Entity = nullptr;
};

