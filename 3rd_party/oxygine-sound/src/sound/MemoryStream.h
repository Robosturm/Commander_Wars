#pragma once
#include <stddef.h>
#include "oxygine/core/FileSystem.h"

namespace oxygine
{
    class fileRMem : public file::fileHandle
    {
    public:
        typedef void(*delfn)(void*);
        static void notdel(void*) {}
        static void delcpp(void* ptr) { delete[](char*)ptr; }

        typedef void(*delself)(fileRMem*);
        static void notdelself(fileRMem*) {}

#if OXYGINE_VERSION < 6
        fileRMem(const void* data, size_t size, delfn fn = notdel, delself fns = notdelself);
#else
        fileRMem(file::FileSystem* fs, const void* data, size_t size, delfn fn = notdel, delself fns = notdelself);
#endif


        ~fileRMem() { release(); }

        void init(const void* data, size_t size, delfn fn = notdel, delself fns = notdelself);

        void release() override;
        unsigned int read(void* dest, unsigned int size) override;
        unsigned int write(const void* src, unsigned int size) override;
        unsigned int getSize() const override;
        int seek(unsigned int offset, int whence) override;
        unsigned int tell() const override;

    protected:
        const void* _data;
        size_t _size;
        size_t _pos;
        delfn _delete;
        delself _deleteSelf;
    };

}
