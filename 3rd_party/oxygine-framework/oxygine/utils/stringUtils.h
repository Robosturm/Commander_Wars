#pragma once
#include "../oxygine-include.h"
#include "../math/Color.h"
#include <stdarg.h>
#include <string>

namespace oxygine
{
    std::string lower(const std::string& str);

    /**str - should be int RGB or RGBA hex format*/
    Color       hex2color(const char* str);
    std::string color2hex(const Color&);

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
