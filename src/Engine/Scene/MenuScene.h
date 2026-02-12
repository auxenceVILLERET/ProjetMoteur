#pragma once
#include "Engine/Scene/Scene.h"

class MenuScene : public Scene
{
public:
	~MenuScene() = default;
	void OnEnter() override;
	void OnExit() override;
	void Update(float dt) override;
};

