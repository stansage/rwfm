#include "stdafx.h"
#include "precisetimer.hpp"

PreciseTimer::PreciseTimer()
{
        QueryPerformanceFrequency(&mTimeout);

	mFrequency = mTimeout.QuadPart / 1000.0;
	QueryPerformanceCounter(&mTimeout);
}


void PreciseTimer::wait(int timeout)
{
        Sleep(timeout)
}

int PreciseTimer::elapsed()
{
	const auto start = mTimeout;
	QueryPerformanceCounter(&mTimeout);
	
	const auto result = (mTimeout.QuadPart - start.QuadPart) / mFrequency;
	mTimeout = start;
	
	return result;
}
