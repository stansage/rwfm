#pragma once

class PreciseTimer
{
public:
	PreciseTimer();
        virtual ~PreciseTimer() = default;

	void wait(int timeout);
	int elapsed();

private:
	LARGE_INTEGER mTimeout;
	double mFrequency = 0;
};

