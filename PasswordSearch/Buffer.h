#pragma once

#include <vector>

struct Buffer
{
    using container_t = std::vector<unsigned char>;

    Buffer();
    Buffer(size_t bufferMaxLength);

    size_t      storageUsedUp = 0;
    container_t buffer = {};
};
