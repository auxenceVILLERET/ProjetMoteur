
#include "Engine.h"
#include "Engine/Core/GameTimer.h"


void Engine::Initialize()
{
	m_isRunning = true;
	m_timer = new GameTimer();

}

void Engine::Run()
{
	if(m_InitCallback)
		m_InitCallback();

	m_timer->Reset();
	while (m_isRunning)
	{
		m_timer->Tick();
		float deltaTime = m_timer->DeltaTime();
		m_ecs.Update(deltaTime);

		if (m_updateCallback)
			m_updateCallback();
	}

	if(m_ShutdownCallback)
		m_ShutdownCallback();
}

void Engine::Shutdown()
{

}

float Engine::GetDeltaTime()
{
	return m_timer->DeltaTime();
}
