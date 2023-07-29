#include "PasswordEncryptor.h"

#include <openssl/evp.h>

PasswordEncryptor::PasswordEncryptor(size_t storageSize)
    : m_encryptedData(storageSize), m_encryptingContext(make_EVP_CIPHER_CTX()) {}

void PasswordEncryptor::encrypt(const Buffer& plainText, const unsigned char* key, const unsigned char* iv)
{
    const auto context = m_encryptingContext.get();
    EVP_EncryptInit_ex(context, EVP_aes_128_cbc(), NULL, key, iv);

    int encrypted = 0;
    EVP_EncryptUpdate(context, &m_encryptedData.buffer[0], &encrypted,
        &plainText.buffer[0], plainText.storageUsedUp);

    int encryptedFinalPart = 0;
    EVP_EncryptFinal_ex(context, &m_encryptedData.buffer[0] + encrypted, &encryptedFinalPart);

    m_encryptedData.storageUsedUp = encrypted + encryptedFinalPart;
}
