#include "Stats.h"
#include "Constants.h"
#include "ArgumentParser.h"
#include "TimeMeasurement.h"
#include "ConsoleNavigator.h"
#include "PasswordSearcher.h"

#include <iostream>
#include <string_view>
#include <mutex>

std::mutex g_consoleMutex;


int main(int argc, char** argv)
{
    TimeMeasurement speedMeasurmentStart;

	std::ios_base::sync_with_stdio(false);

    ArgumentParser applicationInput(argc, argv);
	if (applicationInput.notEnoughArguments())
	{
		std::cout << "No file name in arguments!. Terminating\n";
		system("pause");
		return 0;
	}
    
    PasswordSearcher evpAes128Cbc(applicationInput.getCipherTextFileName(), applicationInput.getLoggerFileName());
    if (evpAes128Cbc.isValid() && evpAes128Cbc.isCipherFileOpened())
    {
		g_consoleMutex.lock();
		std::cout << "File <" << argv[1] << "> successfully read\n";
		g_consoleMutex.unlock();
    }
	else
	{
		g_consoleMutex.lock();
		std::cout << "Can not open file <" << argv[1] << ">. Terminating\n";
        g_consoleMutex.unlock();
		return EXIT_FAILURE;
	}

    ConsoleNavigator::clearScreen();
    evpAes128Cbc.launchSearch();

    const auto secondsElapsed = speedMeasurmentStart.getElapsedTimeCount<Stats::Seconds_t>();

    g_consoleMutex.lock();
    std::cout << "Time difference = " << secondsElapsed << "[s]\n";
    g_consoleMutex.unlock();

    ConsoleNavigator::navigateTo(NavigationPoints::finish);
    return 0;
}
