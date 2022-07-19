#include "Utils.h"

#include <iostream>
#include <fstream>
#include <thread>
#include <cmath>

#include "ConsoleNavigator.h"

bool ReadFile(const char* filePath, std::vector<unsigned char>& outFileBuffer)
{
    std::basic_ifstream<unsigned char> inputFile(filePath, std::ios::binary | std::fstream::in);
    if (!inputFile.is_open())
    {
        return false;
    }

    const auto fileBegin = std::istreambuf_iterator<unsigned char>(inputFile);
    const auto fileEnd = std::istreambuf_iterator<unsigned char>();

    outFileBuffer.clear();
    outFileBuffer.insert(outFileBuffer.begin(), fileBegin, fileEnd);

    return !!inputFile;
}

bool WriteFile(const std::string& filePath, const unsigned char* buffer, size_t bufferSize)
{
    std::basic_ofstream<unsigned char> outputFile(filePath, std::ios::binary);
    if (!outputFile.is_open())
    {
        return false;
    }

    outputFile.write(&buffer[0], bufferSize);
    return !!outputFile;
}

bool WriteFile(const std::string& filePath, const std::vector<unsigned char>& buffer)
{
	std::basic_ofstream<unsigned char> outputFile(filePath, std::ios::binary);
	if (!outputFile.is_open())
	{
		return false;
	}

	outputFile.write(&buffer[0], buffer.size());
	return !!outputFile;
}

bool AppendToFile(const std::string& filePath, const std::vector<unsigned char>& buffer)
{
    std::basic_ofstream<unsigned char> outputFile(filePath, std::ios::binary | std::ios::app);
    if (!outputFile.is_open())
    {
        return false;
    }
    outputFile.write(&buffer[0], buffer.size());
    return !!outputFile;
}

size_t getOptimalThreadCount()
{
	const size_t k_maxThreadsUsed = std::thread::hardware_concurrency();
	return (k_maxThreadsUsed < gsk_minThreadsToUse) ? gsk_minThreadsToUse : k_maxThreadsUsed;
}

std::vector<short int> getAlphabetBasedGroupOfSearchIndexes(size_t workerThreadsCount)
{
	std::vector<short int> searchIndexes(workerThreadsCount);

	const auto partSize = static_cast<short int>(gsk_cipherAlphabetSize / (workerThreadsCount - 1));
	const auto signedIndexesSize = static_cast<short int>(workerThreadsCount);

	for (short int i = 0; i < signedIndexesSize; ++i)
	{
		searchIndexes[i] = (partSize * i) - 1;
	}

	searchIndexes.front() = 0;
	searchIndexes.back() = 0;

	return searchIndexes;
}

void printPasswordToConsole(std::mutex& consoleMutex, const std::string& threadInfo, const std::string& password)
{
	consoleMutex.lock();
	ConsoleNavigator::navigateTo(NavigationPoints::result);
	std::cout << threadInfo << "Found password <" << password << ">\n";
	consoleMutex.unlock();
}

bool isPasswordSucceed(const std::vector<unsigned char>& chipherText, const Buffer& decodedBuf,
	KeyIvGenerator& keyIv, PasswordDecryptor& aes128CbcDecryptor, PasswordEncryptor& aes128CbcEncryptor,
	HashSha256& sha256)
{
	keyIv.updateKeyIv();
	if (!aes128CbcDecryptor.decrypt(chipherText, keyIv.getKeyData(), keyIv.getIvData()))
	{
		return false;
	}

	aes128CbcEncryptor.encrypt(decodedBuf, keyIv.getKeyData(), keyIv.getIvData());
	if (!aes128CbcEncryptor.equalTo(&chipherText[0]))
	{
		return false;
	}

	sha256.calculateWith(decodedBuf);
	return sha256.equalTo(&chipherText[aes128CbcEncryptor.getEncodedLength()]);
}

long long getAllPasswordCombinationsCount(size_t maxPasswordLength)
{
	long long combinations = 0;
	for (size_t i = 0; i < maxPasswordLength; ++i)
	{
		combinations += static_cast<long long>(std::pow(gsk_cipherAlphabet.size(), i + 1));
	}
	return combinations;
}

void printCrypticContextCreationError(std::mutex& consoleMutex, const std::string& threadInfo)
{
	const std::lock_guard<std::mutex> lock(consoleMutex);
	std::cout << threadInfo << "Can`t create cryptic context. Terminating.\n";
}

bool isCrypticContextValid(const PasswordDecryptor& decryptor, const PasswordEncryptor& encryptor) noexcept
{
	return !decryptor.isContextValid() || !encryptor.isContextValid();
}
