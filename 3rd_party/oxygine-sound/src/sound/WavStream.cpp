#include "WavStream.h"
#include <algorithm>

namespace oxygine
{
    struct  WAV_HEADER
    {
        char                RIFF[4];
        uint32_t             ChunkSize;
        char                WAVE[4];
        char                fmt[4];
        uint32_t             Subchunk1Size;
        uint16_t             AudioFormat;
        uint16_t             NumOfChan;
        uint32_t             SamplesPerSec;
        uint32_t             bytesPerSec;
        uint16_t             blockAlign;
        uint16_t             bitsPerSample;
        char                Subchunk2ID[4];
        uint32_t             Subchunk2Size;
    };

#if OXYGINE_VERSION < 6
    WavStream::WavStream() : _fh(0), _close(false), _dataSize(0), _memfile(0, 0)
#else
    WavStream::WavStream() : _fh(0), _close(false), _dataSize(0), _memfile(0, 0, 0)
#endif
    {

    }

    WavStream::~WavStream()
    {
        release();
    }

    void WavStream::release()
    {
        if (_close && _fh)
            file::close(_fh);
        _fh = 0;
    }

    bool WavStream::init(file::handle fh, bool close)
    {
        _fh = fh;
        _close = close;

        WAV_HEADER header;
        file::read(fh, &header, sizeof(header));
        _rate = header.SamplesPerSec;
        _channels = header.NumOfChan;

        _duration = int((int64_t(header.Subchunk2Size) * 1000) / header.bytesPerSec);
        _pcm = header.Subchunk2Size  / _channels / 2;
        _dataSize = header.Subchunk2Size;
        _dataPos = 0;
        OX_ASSERT(header.Subchunk2Size == _pcm * _channels * 2);
        _ended = false;

        return true;
    }

    bool  WavStream::init(const void* data, size_t size)
    {
        _memfile.init(data, size);
        return init(&_memfile, false);
    }

    void WavStream::init(const char* name)
    {
        file::handle fh = file::open(name, "srb");
        init(fh, true);
    }

    int WavStream::decodeNextBlock(bool looped, void* data_, int bufferSize)
    {
        char* data = (char*)data_;
        int total = bufferSize;
        while (bufferSize > 0)
        {
            int leftSize = _dataSize - _dataPos;
            int bytes2read = std::min(leftSize, bufferSize);
            int r = file::read(_fh, data, bytes2read);
            data += bytes2read;
            _dataPos += bytes2read;
            bufferSize -= bytes2read;

            if (bytes2read == leftSize)
            {
                if (looped)
                {
                    file::seek(_fh, sizeof(WAV_HEADER), SEEK_SET);
                    _dataPos = 0;
                }
                else
                {
                    _ended = true;
                    break;
                }
            }
        }

        return total;
    }

    void WavStream::decodeAll(void* data, int bufferSize)
    {
        file::read(_fh, data, bufferSize);
        int p = file::tell(_fh);
        _dataPos = _dataSize;
        _ended = true;
    }

    void WavStream::reset()
    {
        file::seek(_fh, sizeof(WAV_HEADER), SEEK_SET);
        _dataPos = 0;
        _ended = false;
    }

    void WavStream::setPosition(int tm)
    {
        int  p = int(int64_t(tm) * 2 * _channels * _rate / 1000);
        _dataPos = std::min(p, _dataSize);
        file::seek(_fh, sizeof(WAV_HEADER) + _dataPos, SEEK_SET);
    }


    int WavStream::getPosition() const
    {
        int p = int((int64_t(_dataPos) * 1000) / _rate / _channels / 2);
        return p;
    }

}