#pragma once

class PreciseTimer
{
public:
	PreciseTimer();
	virtual ~PreciseTimer();

	void wait(int timeout);
	int elapsed();

private:
	static void CALLBACK timerProc(UINT id, UINT msg, DWORD_PTR user, DWORD_PTR param1, DWORD_PTR param2);

private:
	CRITICAL_SECTION mCS;
	LARGE_INTEGER mTimeout;
	MMRESULT mTimer = 0;
	volatile bool mLeave = false;
	volatile int mCounter = - 1;
	double mFrequency = 0;
};

