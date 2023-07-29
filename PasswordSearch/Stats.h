#pragma once

#include "TimeMeasurement.h"

#include <iostream>
#include <iomanip>
#include <chrono>

class Stats
{
public:
	using rep_t = double;
	using Seconds_t = std::chrono::duration<rep_t>;
	using Milliseconds_t = std::chrono::duration<rep_t, std::milli>;

public:
	Stats(long long allPasswordCombinationsCount);
	void calculate(long long passwordsAlreadyChecked);

	friend std::ostream& operator<<(std::ostream& os, const Stats& calculatedStats);

private:
	const long long m_allPasswordCombinations;
	const size_t m_cachedFieldWidth;

private:
	double m_secondsElapsed;
	double m_currentCheckedPercentage;
	double m_decryptSpeed;
	double m_passwordsAlreadyChecked;
	TimeMeasurement m_startTimePoint;
};