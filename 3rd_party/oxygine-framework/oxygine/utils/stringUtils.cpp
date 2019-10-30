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
}
