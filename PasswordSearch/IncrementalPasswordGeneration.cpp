#include "IncrementalPasswordGeneration.h"
#include <cassert>

IncrementalPasswordGeneration::IncrementalPasswordGeneration(const std::string_view& cipherAlphabet, 
	size_t passwordSize, short int searchGroupBeginIndexInAlphabet,
	short int searchGroupEndIndexInAlphabet, bool isCumulativeGeneration) 
:
	m_searchPasswordSize(passwordSize),
	m_searchGroupEndIndexInAlphabet(searchGroupEndIndexInAlphabet),
	
	m_cipherAlphabet(cipherAlphabet),
	m_signedCipherAlphabetSize(static_cast<int>(cipherAlphabet.size())),

	m_passwordModelSize(gsk_maxBruteForcePasswordLength + sk_modelBufferExtraFrontLength),
	m_passwordModelCachedLastIndex(gsk_maxBruteForcePasswordLength),

	m_passwordModel(),
	m_passwordView()
{
	assert((m_searchPasswordSize <= gsk_maxBruteForcePasswordLength) &&
			"Search password size is more than password buffer");

	if (isCumulativeGeneration)
	{
		this->cumulativeIncrementalInit();
	}
	else
	{
		this->incrementalInit(searchGroupBeginIndexInAlphabet);
	}
}

bool IncrementalPasswordGeneration::generateNextPassword()
{
	incrementModel();
	const size_t symolsToUpdate = updateIncrementedModel();
	updateView(symolsToUpdate);
	return canGenerateNextPassword();
}

void IncrementalPasswordGeneration::incrementalInit(short int searchGroupBeginIndexInAlphabet)
{
	m_passwordModel.assign(gsk_maxBruteForcePasswordLength + sk_modelBufferExtraFrontLength, 0);
	m_passwordModel[m_passwordModelSize - m_searchPasswordSize] = searchGroupBeginIndexInAlphabet;
	m_passwordModel[0] = sk_searchNotFinishedState;

	m_passwordView.buffer.assign(m_passwordModel.size(), 0);
	m_passwordView.storageUsedUp = m_searchPasswordSize;
	for (size_t i = 0; i < m_passwordModel.size(); ++i)
	{
		m_passwordView.buffer[i] = m_cipherAlphabet[m_passwordModel[i]];
	}
}

void IncrementalPasswordGeneration::cumulativeIncrementalInit()
{
	m_passwordModel.assign(gsk_maxBruteForcePasswordLength + sk_modelBufferExtraFrontLength, -1);
	m_passwordModel.front() = sk_searchNotFinishedState;

	m_passwordView.buffer.assign(m_passwordModelSize, 0);
	m_passwordView.storageUsedUp = 1;
}
