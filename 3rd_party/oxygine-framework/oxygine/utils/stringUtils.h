#pragma once
#include "../oxygine-include.h"
#include <stdarg.h>
#include <string>

namespace oxygine
{
    std::string lower(const std::string& str);

    template <size_t N>
    inline
    size_t safe_sprintf(char (&str)[N] , const char* format, ...)
    {
        va_list args;
        va_start(args, format);
        size_t r = vsnprintf(str, N, format, args);
        va_end(args);
        return r;
    }
}
