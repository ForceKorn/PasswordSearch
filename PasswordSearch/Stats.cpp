#include "Stats.h"

Stats::Stats(long long passwordCombinations) 
:
	m_allPasswordCombinations(passwordCombinations),
	m_cachedFieldWidth(std::to_string(passwordCombinations).size()),
	m_secondsElapsed(0.0),
	m_currentCheckedPercentage(0.0),
	m_decryptSpeed(0.0),
	m_passwordsAlreadyChecked(0.0),
	m_startTimePoint()
{}

void Stats::calculate(long long passwordsAlreadyChecked)
{
	const auto currentTimePoint = TimeMeasurement::getCurrentTimePoint();

	m_passwordsAlreadyChecked = static_cast<double>(passwordsAlreadyChecked);
	m_secondsElapsed = m_startTimePoint.getElapsedTimeCount<Seconds_t>(currentTimePoint);
	m_currentCheckedPercentage = (m_passwordsAlreadyChecked / m_allPasswordCombinations) * 100.0;
	m_decryptSpeed = m_passwordsAlreadyChecked / (m_secondsElapsed + 0.001);
}

std::ostream& operator<<(std::ostream& os, const Stats& calculated)
{
	const auto ostreamWidth = std::setw(calculated.m_cachedFieldWidth);

	os << ostreamWidth << std::right << static_cast<int>(calculated.m_passwordsAlreadyChecked) << " from "
		<< calculated.m_allPasswordCombinations << " passwords checked["
		<< static_cast<int>(calculated.m_currentCheckedPercentage) << "%]\n";

	os << "Time elapsed : " << calculated.m_secondsElapsed << "s          \n";
	os << "Speed : " << ostreamWidth << static_cast<int>(calculated.m_decryptSpeed) << " pass/sec\n";
	return os;
}
