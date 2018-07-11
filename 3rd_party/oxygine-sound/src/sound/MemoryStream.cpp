#include "MemoryStream.h"
#include <stdio.h>
#include <string.h>
#include <algorithm>

namespace oxygine
{
#if OXYGINE_VERSION < 6
    fileRMem::fileRMem(const void* data, size_t size, delfn del, delself fns) : _data(data), _size(size), _pos(0), _delete(del), _deleteSelf(fns)
#else
    fileRMem::fileRMem(file::FileSystem * fs, const void* data, size_t size, delfn del, delself fns) : fileHandle(fs), _data(data), _size(size), _pos(0), _delete(del), _deleteSelf(fns)
#endif
    {

    }

    void fileRMem::init(const void* data, size_t size, delfn fn /*= notdel*/, delself fns /*= notdelself*/)
    {
        release();
        _data = data;
        _size = size;
        _delete = fn;
        _deleteSelf = fns;
    }

    void fileRMem::release()
    {
        _delete(const_cast<void*>(_data));
        _data = 0;
        _size = 0;
        _pos = 0;
        _deleteSelf(this);
    }

    unsigned int fileRMem::read(void* dest, unsigned int size)
    {
        unsigned int bytes = std::min(size, (unsigned int)(_size - _pos));
        memcpy(dest, (const char*)_data + _pos, bytes);
        _pos += bytes;
        return bytes;
    }

    unsigned int fileRMem::write(const void* src, unsigned int size)
    {
        return 0;
    }

    unsigned int fileRMem::getSize() const
    {
        return _size;
    }

    int fileRMem::seek(unsigned int offset, int whence)
    {
        switch (whence)
        {
            case SEEK_CUR:
            {
                size_t pos = _pos + offset;
                if (pos > _size)
                    return -1;
                _pos += offset;
            }
            break;
            case SEEK_END:
            {
                OX_ASSERT(offset == 0);
                _pos = _size;
                return 0;
            }
            break;
            case SEEK_SET:
            {
                if (offset > _size)
                    return -1;

                _pos = offset;
            }
            break;

            default:
                OX_ASSERT(0);
                break;
        }

        return 0;
    }

    unsigned int fileRMem::tell() const
    {
        return _pos;
    }

}
