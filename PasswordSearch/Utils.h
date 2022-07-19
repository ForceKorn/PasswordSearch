#pragma once

#include <vector>
#include <string>
#include <mutex>

#include "Buffer.h"
#include "PasswordEncryptor.h"
#include "PasswordDecryptor.h"
#include "KeyIvGenerator.h"
#include "HashSha256.h"


bool ReadFile(const char* filePath, std::vector<unsigned char>& outFileBuffer);
bool WriteFile(const std::string& filePath, const std::vector<unsigned char>& buffer);
bool WriteFile(const std::string& filePath, const unsigned char* buffer, size_t bufferSize);
bool AppendToFile(const std::string& filePath, const std::vector<unsigned char>& buffer);

auto getAllPasswordCombinationsCount(size_t maxPasswordLength) -> long long;
auto getOptimalThreadCount() -> size_t;
auto getAlphabetBasedGroupOfSearchIndexes(size_t workerThreadsCount) -> std::vector<short int>;

void printCrypticContextCreationError(std::mutex& consoleMutex, const std::string& threadInfo);
void printPasswordToConsole(std::mutex& consoleMutex, const std::string& threadInfo, const std::string& password);

bool isCrypticContextValid(const PasswordDecryptor& decryptor, const PasswordEncryptor& encryptor) noexcept;
bool isPasswordSucceed(const std::vector<unsigned char>& chipherText, const Buffer& decodedBuf, 
	KeyIvGenerator& keyIv, PasswordDecryptor& aes128CbcDecryptor, PasswordEncryptor& aes128CbcEncryptor, HashSha256& sha256);
