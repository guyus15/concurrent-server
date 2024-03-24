#include "ui/ui_helper.h"

#include <cstddef>

bool HasContent(const char* string)
{
    size_t size = 0;
    for (size; *(string + size) != '\0'; size++)
    {
    }

    return size > 0;
}
