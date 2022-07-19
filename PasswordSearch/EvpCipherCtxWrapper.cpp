#include "EvpCipherCtxWrapper.h"


EVP_CIPHER_CTX_WRAPPER::EVP_CIPHER_CTX_WRAPPER(EVP_CIPHER_CTX* cryptionContext) 
    : m_context(cryptionContext) {}

EVP_CIPHER_CTX_WRAPPER::~EVP_CIPHER_CTX_WRAPPER()
{
    if (m_context != 0)
    {
        EVP_CIPHER_CTX_free(m_context);
    }
}

EVP_CIPHER_CTX_WRAPPER make_EVP_CIPHER_CTX()
{
    return EVP_CIPHER_CTX_new();
}
