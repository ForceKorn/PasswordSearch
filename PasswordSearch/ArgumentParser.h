#pragma once

class ArgumentParser
{
public:
	ArgumentParser(int argc, char** argv);

	inline bool notEnoughArguments() const noexcept
	{
		return m_argc < 2;
	}

	inline const char* getCipherTextFileName() const noexcept
	{
		return m_argv[1];
	}

	const char* getLoggerFileName() const noexcept;

private:
	int     m_argc = 0;
	char** m_argv = nullptr;
};
