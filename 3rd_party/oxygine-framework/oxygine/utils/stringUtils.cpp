#include "stringUtils.h"
#include "../core/Object.h"
#include <algorithm>
#include <string.h>

namespace oxygine
{
    namespace path
    {
        bool unitTest()
        {
            char dest[255];

            //normalize("//", dest);
            //Q_ASSERT(strcmp(dest, "/") == 0);

            normalize("ab/cd/../qw", dest);
            Q_ASSERT(strcmp(dest, "ab/qw") == 0);
            normalize("ab/cd/ef/../../qw", dest);
            Q_ASSERT(strcmp(dest, "ab/qw") == 0);


            normalize("rom://abc", dest);
            Q_ASSERT(strcmp(dest, "rom://abc") == 0);

            normalize("/../", dest);
            Q_ASSERT(strcmp(dest, "/../") == 0);

            normalize("../c\\", dest);
            Q_ASSERT(strcmp(dest, "../c/") == 0);

            normalize("a//", dest);
            Q_ASSERT(strcmp(dest, "a/") == 0);


            normalize("\\/\\///\\/", dest);// \n
            Q_ASSERT(strcmp(dest, "/") == 0);

            normalize("a/b/..\\//..///\\/../c\\\\/", dest);
            Q_ASSERT(strcmp(dest, "../c/") == 0);

            normalize("a/../c", dest);
            Q_ASSERT(strcmp(dest, "c") == 0);

            normalize("a/b/../../../c", dest);
            Q_ASSERT(strcmp(dest, "../c") == 0);

            normalize("..a/b/..\\//..///\\/../c\\\\/", dest);
            Q_ASSERT(strcmp(dest, "../c/") == 0);

            return true;
        }

        void reverse(char* str)
        {
            int len = (int)strlen(str);
            int half = len / 2;
            for (int i = 0; i < half; ++i)
            {
                std::swap(str[i], str[len - i - 1]);
            }
        }

        void split(const char* src, char* destHead, char* destTail)
        {
            Q_ASSERT(src != destHead);
            Q_ASSERT(src != destTail);

            destHead[0] = 0;
            destTail[0] = 0;

            int i = 0;
            int len = (int)strlen(src);
            while (len)
            {
                char c = src[len - 1];
                if (c == '\\' || c == '/')
                {
                    break;
                }

                destTail[i] = c;
                ++i;
                --len;
            }
            destTail[i] = 0;
            reverse(destTail);

            memcpy(destHead, src, len);
            destHead[len] = 0;
        }

        void splitExt(const char* src, char* destHead, char* destTail)
        {
            Q_ASSERT(src != destHead);
            Q_ASSERT(src != destTail);

            destHead[0] = 0;
            destTail[0] = 0;

            int len = (int)strlen(src);
            int i = 0;
            for (i = len - 1; i >= 0; --i)
            {
                if (src[i] == '.')
                    break;
            }

            if (i == -1)
                i = len;
            memcpy(destHead, src, i);
            destHead[i] = 0;

            int n = len - i;
            memcpy(destTail, src + i, n);
            destTail[n] = 0;
        }

        void normalize(const char* src, char* dest)
        {
            Q_ASSERT(src != dest);

            if (*src == '/' || *src == '\\')
            {
                *dest = '/';
                ++src;
                ++dest;
            }
            char* copy = dest;

            *dest = 0;
            while (*src)
            {
                char c = *src;
                char last =  dest - 1 >= copy ? *(dest - 1) : '/';
                char last_last =  dest - 2 >= copy ? *(dest - 2) : '/';

                if (c == '\\')
                    c = '/';
                if (c == ':')
                {
                    *dest++ = *src++;
                    *dest++ = *src++;
                    *dest++ = *src++;
                    continue;
                }
                if (c == '/' && last == '.' && last_last == '.' && (dest > copy + 2))
                {
                    dest -= 3;
                    while (dest > copy)
                    {
                        --dest;
                        if (*dest == '/')
                        {
                            ++dest;
                            break;
                        }
                    }

                    ++src;
                    continue;
                }
                if (last == c && c == '/')
                {
                    ++src;
                    continue;
                }

                *dest = c;

                //*(dest + 1) = 0;
                //qDebug(copy);

                ++src;
                ++dest;

            }
            *dest = 0;
        }

        std::string normalize(const std::string& pth)
        {
            char path[512];
            normalize(pth.c_str(), path);
            return path;
        }

        std::string extractFileName(const std::string& path)
        {
            size_t pos = path.find_last_of("\\/") + 1;
            std::string name = path.substr(pos, path.size() - pos);
            return name;
        }

        std::string extractBaseFileName(const std::string& filename)
        {
            size_t pos = filename.find_last_of('.');
            if (pos == std::string::npos)
                pos = filename.size();
            std::string name = filename.substr(0, pos);
            return name;
        }

        std::string extractFileExt(const std::string& filename)
        {
            size_t pos = filename.find_last_of(".");
            if (pos == std::string::npos)
                return "";
            pos += 1;
            std::string name = filename.substr(pos, filename.size() - pos);
            return name;
        }

        std::string extractFolder(const std::string& path)
        {
            size_t pos = path.find_last_of("\\/") + 1;
            std::string name = path.substr(0, pos);
            return name;
        }
    }


    const char* getNextCode(int& code, const char* utf8str)
    {
        const char* utfstr = utf8str;
        code = 0;
        char* p = (char*)&code;

        const char& c = *utfstr++;
        p[0] = c;

        if ((c & 0xE0) == 0xE0)
        {
            p[1] = *utfstr++;
            p[2] = *utfstr++;
        }
        else if ((c & 0xC0) == 0xC0)
        {
            p[1] = *utfstr++;
        }

        return utfstr;
    }

    void charCode2Bytes(std::string& res, int code)
    {
        const char* p = (char*)&code;
        const char& c = *p;

        res.push_back(*p++);
        if ((c & 0xE0) == 0xE0)
        {
            res.push_back(*p++);
            res.push_back(*p++);
        }
        else if ((c & 0xC0) == 0xC0)
        {
            res.push_back(*p++);
        }
    }

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

    int strcmp_cns(const char* fileName1, const char* fileName2)
    {
        for (;;)
        {
            char c1 = *(fileName1++);
            char c2 = *(fileName2++);
            if ((c1 >= 'a') && (c1 <= 'z'))
                c1 -= 0x20;
            if ((c2 >= 'a') && (c2 <= 'z'))
                c2 -= 0x20;
            if (c1 == '\0')
                return ((c2 == '\0') ? 0 : -1);
            if (c2 == '\0')
                return 1;
            if (c1 < c2)
                return -1;
            if (c1 > c2)
                return 1;
        }
        return 1;
    }
}
