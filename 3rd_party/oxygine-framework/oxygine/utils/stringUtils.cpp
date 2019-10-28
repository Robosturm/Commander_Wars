#include "stringUtils.h"
#include "../core/Object.h"
#include <algorithm>
#include <string.h>

namespace oxygine
{
    std::string lower(const std::string& str)
    {
        std::string data = str;
        std::transform(data.begin(), data.end(), data.begin(), ::tolower);//todo optimize
        return data;
    }

    Color   hex2color(const char* str)
    {
        int len = (int)strlen(str);
        if (len >= 6)
        {

            int rgba = 0;
            sscanf(str, "%x", &rgba);
            if (len == 6)
                rgba = (rgba << 8) |  0xff;

            return Color(rgba);
        }

        return Color(0xffffffff);
    }

    std::string color2hex(const Color& c)
    {
        char str[255];
        safe_sprintf(str, "%02x%02x%02x%02x", c.r, c.g, c.b, c.a);
        return std::string(str);
    }
}
