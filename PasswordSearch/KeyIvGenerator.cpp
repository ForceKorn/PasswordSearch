#include "KeyIvGenerator.h"

#include <cmath>

KeyIvGenerator::KeyIvGenerator(size_t expectedPasswordSize, short int searchGroupBeginIndexInAlphabet,
	short int searchGroupEndIndexInAlphabet, bool isCumulativeGeneration)
	: m_generationStrategy(gsk_cipherAlphabet, expectedPasswordSize, 
		searchGroupBeginIndexInAlphabet, searchGroupEndIndexInAlphabet, isCumulativeGeneration) 
{}

bool KeyIvGenerator::makeKeyIvWithPassword() noexcept
{
	return !!EVP_BytesToKey(EVP_aes_128_cbc(),
		EVP_md5(),
		0,
		reinterpret_cast<const unsigned char*>(m_generationStrategy.getPasswordBegin()),
		m_generationStrategy.getPasswordSize(),
		1,
		getKeyData(),
		getIvData());
}
