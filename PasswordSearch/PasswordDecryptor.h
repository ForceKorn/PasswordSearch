#pragma once

#include "Buffer.h"
#include "EvpCipherCtxWrapper.h"

#include <vector>

class PasswordDecryptor
{
public:
    PasswordDecryptor(size_t storageSize);
    
    bool decrypt(const std::vector<unsigned char>& cipherData, const unsigned char* key, 
        const unsigned char* iv);

    inline bool isContextValid() const noexcept
    {
        return !!m_decryptingContext.get();
    }

    inline size_t getDecodedLength() const noexcept
    {
        return m_decryptedData.storageUsedUp;
    }

    inline const Buffer& getBuffer() const noexcept
    {
        return m_decryptedData;
    }

private:
    Buffer m_decryptedData;
    EVP_CIPHER_CTX_WRAPPER m_decryptingContext;
};
