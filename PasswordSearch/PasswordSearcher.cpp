#include "PasswordSearcher.h"

#include "Utils.h"
#include "Stats.h"
#include "Constants.h"
#include "ConsoleNavigator.h"
#include "Buffer.h"
#include "PasswordDecryptor.h"
#include "PasswordEncryptor.h"
#include "KeyIvGenerator.h"
#include "HashSha256.h"

#include <iostream>

PasswordSearcher::PasswordSearcher(const char* encryptedFileName, const std::string_view& loggerFileName)
:
    m_workers(getOptimalThreadCount()),
	m_fileLogger(nullptr),
	m_chipherText(),
	m_passwordsCurentlyChecked(0),
	m_isPasswordFound(false),
	m_fileOpened(false)
{
	m_fileOpened = ReadFile(encryptedFileName, m_chipherText);
	createLogger(loggerFileName);
}

void PasswordSearcher::launchSearch()
{
	createWorkers();

	for (auto& workerThread : m_workers)
	{
		workerThread.join();
	}
}

bool PasswordSearcher::isValid() const noexcept
{
	return !m_chipherText.empty();
}

bool PasswordSearcher::isCipherFileOpened() const noexcept
{
	return m_fileOpened;
}

void PasswordSearcher::printStatsToConsole()
{
	if (!ConsoleNavigator::isValid()) { return; }

	Stats progress(getAllPasswordCombinationsCount(gsk_CurrentPasswordLength));
	while (!m_isPasswordFound)
	{
		progress.calculate(m_passwordsCurentlyChecked);
		ConsoleNavigator::navigateTo(NavigationPoints::stats);

		g_consoleMutex.lock();
		std::cout << progress;
		g_consoleMutex.unlock();

		std::this_thread::sleep_for(gsk_printSleepTime);
	}
}

void PasswordSearcher::createLogger(const std::string_view& loggerFileName)
{
	if (loggerFileName.empty())
	{
		m_fileLogger = std::make_unique<EmptyLogger>();
	}
	else
	{
		m_fileLogger = std::make_unique<FileLogger>(loggerFileName.data());
	}
}

void PasswordSearcher::createWorkers()
{	
	const std::vector<short int> searchIndexes = {
		getAlphabetBasedGroupOfSearchIndexes(m_workers.size())
	};

	m_workers[0] = std::thread(&PasswordSearcher::printStatsToConsole, this);
	m_workers[1] = std::thread(&PasswordSearcher::passwordSearch, this,
		searchIndexes[0], searchIndexes[1], true);

	for (size_t i = 2; i < m_workers.size(); ++i)
	{
		m_workers[i] = std::thread(&PasswordSearcher::passwordSearch, this,
			searchIndexes[i - 1], searchIndexes[i], false);
	}
}

void PasswordSearcher::passwordSearch(short int searchGroupBeginIndexInAlphabet,
	short int searchGroupEndIndexInAlphabet, bool isCumulativeGeneration)
{
	KeyIvGenerator keyIv(gsk_CurrentPasswordLength, searchGroupBeginIndexInAlphabet, 
		searchGroupEndIndexInAlphabet, isCumulativeGeneration);

	HashSha256 sha256(SHA256_DIGEST_LENGTH);
	PasswordDecryptor aes128CbcDecryptor(m_chipherText.size() - sha256.size());
	PasswordEncryptor aes128CbcEncryptor(m_chipherText.size());

	const Buffer& decodedBuf = aes128CbcDecryptor.getBuffer();
	const Buffer& encodedBuf = aes128CbcEncryptor.getBuffer();

	const std::string threadInfo = std::string("Thread <")
		+ std::to_string(static_cast<long>(GetCurrentThreadId())) + "> -> ";

	if (isCrypticContextValid(aes128CbcDecryptor, aes128CbcEncryptor))
	{
		printCrypticContextCreationError(g_consoleMutex, threadInfo);
		return;
	}

	while (keyIv.hasPasswordsToGenerate())
	{
		logPassword(reinterpret_cast<const char*>(keyIv.getPasswordBeginAddress()), 
						keyIv.getPasswordLength());

		if (m_isPasswordFound) { return; }		

		if (isPasswordSucceed(m_chipherText, decodedBuf, keyIv, aes128CbcDecryptor,
			aes128CbcEncryptor, sha256))
		{
			m_isPasswordFound = true;
			printPasswordToConsole(g_consoleMutex, threadInfo, keyIv.getPassword());
			WriteFile("DecodeData", &decodedBuf.buffer[0], aes128CbcDecryptor.getDecodedLength());
			return;
		}

		++m_passwordsCurentlyChecked;
	}
}
