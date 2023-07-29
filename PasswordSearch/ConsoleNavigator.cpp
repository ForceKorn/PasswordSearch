#include "ConsoleNavigator.h"

#include <vector>

HANDLE ConsoleNavigator::m_console = GetStdHandle(STD_OUTPUT_HANDLE);
std::vector<COORD> locations = 
{
	{ -1, -1},
	{ 0,  2 },
	{ 0,  6 },
	{ 0,  8 }
};

int getLocation(NavigationPoints location)
{
	return static_cast<int>(location);
}

bool ConsoleNavigator::isValid() noexcept
{
	return (m_console != INVALID_HANDLE_VALUE) && (m_console != NULL);
}

void ConsoleNavigator::navigateTo(NavigationPoints location)
{
	SetConsoleCursorPosition(m_console, locations[getLocation(location)]);
}

void ConsoleNavigator::clearScreen() noexcept
{
	system("cls");
}
