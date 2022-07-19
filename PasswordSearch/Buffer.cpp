#include "Buffer.h"

Buffer::Buffer()
	: storageUsedUp(0), buffer() {}

Buffer::Buffer(size_t bufferMaxStorageLength)
    : storageUsedUp(0), buffer(bufferMaxStorageLength, '\0') {}
