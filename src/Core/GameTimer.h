//***************************************************************************************
// GameTimer.h by Frank Luna (C) 2011 All Rights Reserved.
//***************************************************************************************

#ifndef GAMETIMER_H
#define GAMETIMER_H

class GameTimer
{
public:
	GameTimer();

	float TotalTime()const; // in seconds
	float DeltaTime()const; // in seconds

	void Reset(); // Call before message loop.
	void Start(); // Call when unpaused.
	void Stop();  // Call when paused.
	void Tick();  // Call every frame.

private:
	double m_SecondsPerCount;
	double m_DeltaTime;

	int m_BaseTime;
	int m_PausedTime;
	int m_StopTime;
	int m_PrevTime;
	int m_CurrTime;

	bool m_Stopped;
};

#endif // GAMETIMER_H