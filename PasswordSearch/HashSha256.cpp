#include "HashSha256.h"

HashSha256::HashSha256(size_t hashLength) : m_hash(hashLength) {}

void HashSha256::calculateWith(const Buffer& plain)
{
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, &plain.buffer[0], plain.storageUsedUp);
    SHA256_Final(&m_hash[0], &sha256);
}
