#include "stdafx.h"
#include "precisetimer.hpp"

PreciseTimer::PreciseTimer()
{
	InitializeCriticalSection(&mCS);
	mTimer = timeSetEvent(1, 0, &timerProc, reinterpret_cast<DWORD>(this), TIME_PERIODIC);

	QueryPerformanceFrequency(&mTimeout);

	mFrequency = mTimeout.QuadPart / 1000.0;
	QueryPerformanceCounter(&mTimeout);
}


PreciseTimer::~PreciseTimer()
{
	mTimer = timeKillEvent(mTimer);
	DeleteCriticalSection(&mCS);
}

void PreciseTimer::wait(int timeout)
{
	if (timeout != 0) {
		mCounter = timeout;
		mLeave = true;
		EnterCriticalSection(&mCS);
		LeaveCriticalSection(&mCS);
	}
}

int PreciseTimer::elapsed()
{
	const auto start = mTimeout;
	QueryPerformanceCounter(&mTimeout);
	
	const auto result = (mTimeout.QuadPart - start.QuadPart) / mFrequency;
	mTimeout = start;
	
	return result;
}

void PreciseTimer::timerProc(UINT id, UINT msg, DWORD_PTR user, DWORD_PTR param1, DWORD_PTR param2)
{
	static volatile auto entered = false;
	const auto me = reinterpret_cast<PreciseTimer*>(user);

	if (me != nullptr) {
		if (!entered && !me->mLeave) {
			entered = true;
			EnterCriticalSection(&me->mCS);
		} else if (me->mLeave && me->mCounter == 0) {
			me->mLeave = false;
			entered = false;
			LeaveCriticalSection(&me->mCS);
		} else if (me->mCounter > 0) {
			--me->mCounter;
		}
	}
}