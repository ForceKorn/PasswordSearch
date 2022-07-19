#pragma once

#include <openssl/evp.h>

class EVP_CIPHER_CTX_WRAPPER
{
public:
    EVP_CIPHER_CTX_WRAPPER(EVP_CIPHER_CTX* cryptionContext);
    ~EVP_CIPHER_CTX_WRAPPER();

    EVP_CIPHER_CTX_WRAPPER(const EVP_CIPHER_CTX_WRAPPER&) = delete;
    EVP_CIPHER_CTX_WRAPPER& operator=(const EVP_CIPHER_CTX_WRAPPER&) = delete;

    inline EVP_CIPHER_CTX* get() const noexcept
    {
        return m_context;
    }

    inline operator EVP_CIPHER_CTX* () const
    {
        return m_context;
    }

private:
    EVP_CIPHER_CTX* m_context = 0;
};

EVP_CIPHER_CTX_WRAPPER make_EVP_CIPHER_CTX();
