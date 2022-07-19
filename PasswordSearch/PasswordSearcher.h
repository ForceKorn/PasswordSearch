#pragma once

#include "Logger.h"

#include <string_view>
#include <vector>
#include <memory>
#include <thread>
#include <atomic>


extern std::mutex g_consoleMutex;

class PasswordSearcher
{
public:
	PasswordSearcher(const char* encryptedFileName, const std::string_view& loggerFileName);
	
	void launchSearch();
	bool isValid() const noexcept;
	bool isCipherFileOpened() const noexcept;

private:
	void printStatsToConsole();
	void createLogger(const std::string_view& loggerFileName);
	void createWorkers();

	void passwordSearch(short int searchGroupBeginIndexInAlphabet, 
		short int searchGroupEndIndexInAlphabet, bool isCumulativeGeneration);

	inline void logPassword(const char* data, size_t dataLenth)
	{
		m_fileLogger->log(data, dataLenth);
	}

private:
	std::vector<std::thread>	m_workers;
	
	std::unique_ptr<ILogger>	m_fileLogger;
	std::vector<unsigned char>	m_chipherText;
	std::atomic<long long>		m_passwordsCurentlyChecked;
	std::atomic<bool>			m_isPasswordFound;
	bool						m_fileOpened;
};
