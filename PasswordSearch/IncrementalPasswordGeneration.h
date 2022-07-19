#pragma once

#include "Buffer.h"
#include "Constants.h"

#include <string_view>
#include <vector>

class IncrementalPasswordGeneration
{
public:
	IncrementalPasswordGeneration(const std::string_view& cipherAlphabet, size_t passwordSize,
		short int searchGroupBeginIndexInAlphabet, short int searchGroupEndIndexInAlphabet, 
		bool isCumulativeGeneration);

	bool generateNextPassword();
	inline unsigned char* getPasswordBegin();
	inline const unsigned char* getPasswordBegin() const;
	inline size_t getPasswordSize() const;
	
private:
	inline void incrementModel();
	inline size_t updateIncrementedModel();
	inline void updateView(size_t symbolsToUpdate);
	inline bool canGenerateNextPassword() const noexcept;

	void incrementalInit(short int searchGroupBeginIndexInAlphabet);
	void cumulativeIncrementalInit();

private:
	static constexpr size_t		sk_modelBufferExtraFrontLength = 1;
	static constexpr short int  sk_searchNotFinishedState = 0;

private:
	const size_t	m_searchPasswordSize;
	const short int m_searchGroupEndIndexInAlphabet;

	const std::string_view& m_cipherAlphabet;
	const int				m_signedCipherAlphabetSize;

	const size_t m_passwordModelSize;
	const size_t m_passwordModelCachedLastIndex;

private:
	std::vector<short int> m_passwordModel;
	Buffer				   m_passwordView;
};


inline unsigned char* IncrementalPasswordGeneration::getPasswordBegin()
{
	return &m_passwordView.buffer[m_passwordModelSize - m_passwordView.storageUsedUp];
}

inline const unsigned char* IncrementalPasswordGeneration::getPasswordBegin() const
{
	return &m_passwordView.buffer[m_passwordModelSize - m_passwordView.storageUsedUp];
}

inline size_t IncrementalPasswordGeneration::getPasswordSize() const
{
	return m_passwordView.storageUsedUp;
}

inline size_t IncrementalPasswordGeneration::updateIncrementedModel()
{
	size_t count = 1;
	for (int i = m_passwordModelCachedLastIndex; i > 0; --i)
	{
		if (m_passwordModel[i] < m_signedCipherAlphabetSize)
		{
			break;
		}
		else
		{
			m_passwordModel[i] = 0;
			++m_passwordModel[i - 1];
			++count;
		}
	}
	return count;
}

inline void IncrementalPasswordGeneration::updateView(size_t symbolsToUpdate)
{
	for (size_t i = 0; i < symbolsToUpdate; ++i)
	{
		const auto index = m_passwordModelCachedLastIndex - i;
		m_passwordView.buffer[index] = m_cipherAlphabet[m_passwordModel[index]];
	}

	if (m_passwordView.storageUsedUp < symbolsToUpdate)
	{
		m_passwordView.storageUsedUp = symbolsToUpdate;
	}
}

inline bool IncrementalPasswordGeneration::canGenerateNextPassword() const noexcept
{
	return m_passwordModel[m_passwordModelSize - m_searchPasswordSize]
			!= m_searchGroupEndIndexInAlphabet;
}

inline void IncrementalPasswordGeneration::incrementModel()
{
	++m_passwordModel[m_passwordModelCachedLastIndex];
}
