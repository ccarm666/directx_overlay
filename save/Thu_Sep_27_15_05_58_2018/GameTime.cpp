#include "GameTime.h"

bool GameTime::Initialize()
{
	LARGE_INTEGER i;

	//Get frequency from counter.  The frequency cannot change while the system is running, so we only need to do this once.
	if(!QueryPerformanceFrequency(&i))
		return false;

	//Gives us number of ticks per second.
	frequencySeconds = (float)(i.QuadPart);

	//Gets the current value of the counter
	QueryPerformanceCounter(&i);
	start = i.QuadPart;
	totalGameTime = 0;

	return true;
}

void GameTime::Update()
{
	LARGE_INTEGER i;

	QueryPerformanceCounter(&i);
	elapsedTime = (float)(i.QuadPart - start) / frequencySeconds;

	start = i.QuadPart;
	totalGameTime += elapsedTime;
}

