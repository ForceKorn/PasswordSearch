#pragma once

#include <vector>

#include <windows.h>

enum class NavigationPoints : int
{
	error = 0,
	stats,
	result,
	finish,
};

class ConsoleNavigator
{
public:
	static void clearScreen() noexcept;
	static bool isValid() noexcept;
	static void navigateTo(NavigationPoints location);

private:
	static HANDLE m_console;
};
