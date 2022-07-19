#pragma once

#include "Buffer.h"

#include <vector>
#include <cstring>

#include <openssl/sha.h>

class HashSha256
{
public:
    HashSha256(size_t hashLength = SHA256_DIGEST_LENGTH);

    void calculateWith(const Buffer& plainText);

    inline bool equalTo(const unsigned char* cipherTextHash)
    {
        return !memcmp(cipherTextHash, &m_hash[0], m_hash.size());
    }

    inline size_t size() const noexcept
    {
        return m_hash.size();
    }

private:
    std::vector<unsigned char> m_hash;
};

