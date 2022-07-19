#include "ArgumentParser.h"
#include "Constants.h"

#include <string_view>

ArgumentParser::ArgumentParser(int argc, char** argv) : m_argc(argc), m_argv(argv) {}

const char* ArgumentParser::getLoggerFileName() const noexcept
{
	std::string_view logerFileName;
	if (m_argc >= 4)
	{
		const std::string_view loggerKey = m_argv[2];
		const std::string_view loggerValue = m_argv[3];
		const bool isLoggerKeyValid = loggerKey.find(gsk_keyPasswordsLogging)
			!= std::string_view::npos;

		if (isLoggerKeyValid && !loggerValue.empty())
		{
			return m_argv[3];
		}
	}
	return "";
}
