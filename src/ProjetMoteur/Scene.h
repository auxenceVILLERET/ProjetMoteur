#pragma once


class Scene
{
public:
	Scene();
	~Scene();

	bool IsActive();
	void SetActive(bool active);

	virtual void Update();

protected:
	bool m_active = false;
};

