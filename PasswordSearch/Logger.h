#pragma once

#include <fstream>
#include <mutex>

class ILogger
{
public:
	virtual void log(const char* data, size_t dataLength) = 0;
};

class EmptyLogger : public ILogger
{
public:
	void log(const char* data, size_t dataLength) override;
};

class FileLogger : public ILogger
{
public:
	FileLogger(const char* streamFileName);
	void log(const char* data, size_t dataLength) override;

private:
	std::ofstream m_stream;
	std::mutex	  m_fileMutex;
};
