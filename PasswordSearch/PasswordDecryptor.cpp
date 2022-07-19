#include "PasswordDecryptor.h"

#include <openssl/evp.h>

PasswordDecryptor::PasswordDecryptor(size_t storageSize)
    : m_decryptedData(storageSize), m_decryptingContext(make_EVP_CIPHER_CTX()) {}

bool PasswordDecryptor::decrypt(const std::vector<unsigned char>& cipher, const unsigned char* key,
    const unsigned char* iv)
{
    const auto context = m_decryptingContext.get();

    EVP_DecryptInit_ex(context, EVP_aes_128_cbc(), NULL, key, iv);

    int decrypted = 0;
    EVP_DecryptUpdate(context, &m_decryptedData.buffer[0], &decrypted, 
        &cipher[0], m_decryptedData.buffer.size());

    int decryptedFinalPart = 0;
    const bool isValid = !!EVP_DecryptFinal_ex(context, &m_decryptedData.buffer[decrypted], 
        &decryptedFinalPart);

    m_decryptedData.storageUsedUp = decrypted + decryptedFinalPart;

    return isValid;
}
