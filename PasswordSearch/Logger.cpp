#include "Logger.h"

void EmptyLogger::log(const char* data, size_t dataLength) {}

FileLogger::FileLogger(const char* streamFileName)
	: m_stream(streamFileName), m_fileMutex() {}

void FileLogger::log(const char* data, size_t dataLength)
{
	const std::lock_guard<std::mutex> lock(m_fileMutex);
	m_stream.write(data, dataLength) << '\n';
}
