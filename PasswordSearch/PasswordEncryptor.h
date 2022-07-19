#pragma once

#include "Buffer.h"
#include "EvpCipherCtxWrapper.h"

#include <cstring>

class PasswordEncryptor
{
public:
    PasswordEncryptor(size_t storageSize);

    void encrypt(const Buffer& plainText, const unsigned char* key, const unsigned char* iv);

    inline bool isContextValid() const noexcept
    {
        return !!m_encryptingContext.get();
    }

    inline size_t getEncodedLength() const noexcept
    {
        return m_encryptedData.storageUsedUp;
    }

    inline const Buffer& getBuffer() const noexcept
    {
        return m_encryptedData;
    }

    inline bool equalTo(const unsigned char* cipherTextHashBegin)
    {
        return !memcmp(cipherTextHashBegin, &m_encryptedData.buffer[0], m_encryptedData.storageUsedUp);
    }

private:
    Buffer m_encryptedData;
    EVP_CIPHER_CTX_WRAPPER m_encryptingContext;
};
