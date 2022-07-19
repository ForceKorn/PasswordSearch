#pragma once

#include "Buffer.h"
#include "IncrementalPasswordGeneration.h"

#include <string>
#include <vector>
#include <array>

#include <openssl/evp.h>


class KeyIvGenerator
{
public:
	KeyIvGenerator(size_t passwordSize, short int searchGroupBeginIndexInAlphabet, 
		short int searchGroupEndIndexInAlphabet, bool isCumulativeGeneration);
	
	inline void updateKeyIv()
	{
		m_cachedHasPasswordsToGenerate = generatePassword();
		makeKeyIvWithPassword();
	}

	inline const unsigned char* getKeyData() const noexcept
	{
		return &m_key[0];
	}

	inline const unsigned char* getIvData() const noexcept
	{
		return &m_iv[0];
	}

	inline unsigned char* getKeyData() noexcept
	{
		return &m_key[0];
	}

	inline unsigned char* getIvData() noexcept
	{
		return &m_iv[0];
	}

	inline std::string getPassword() noexcept
	{
		std::string result(m_generationStrategy.getPasswordSize(), '0');
		std::memcpy(&result[0], m_generationStrategy.getPasswordBegin(), result.size());
		return result;
	}

	inline const unsigned char* getPasswordBeginAddress() const
	{
		return m_generationStrategy.getPasswordBegin();
	}

	inline size_t getPasswordLength() const
	{
		return m_generationStrategy.getPasswordSize();
	}

	inline bool hasPasswordsToGenerate() const
	{
		return m_cachedHasPasswordsToGenerate;
	}

private:
	inline bool generatePassword()
	{
		return m_generationStrategy.generateNextPassword();
	}

	bool makeKeyIvWithPassword() noexcept;

private:
	IncrementalPasswordGeneration m_generationStrategy;

	std::array<unsigned char, EVP_MAX_KEY_LENGTH> m_key = { 0 };
	std::array<unsigned char, EVP_MAX_IV_LENGTH>  m_iv = { 0 };

	bool m_cachedHasPasswordsToGenerate = true;
};
