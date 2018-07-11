#include "OggStream.h"
#include "oxygine/core/file.h"


#if TREMOLO
#include "ogg.h"
#else
#include "ogg/ogg.h"
#endif

#include "oxygine/core/ox_debug.h"

//#include "s3eDevice.h"

#ifndef _WIN32
//typedef ogg_int64_t __int64;
#endif

namespace oxygine
{
    size_t oxfile_ov_read_func(void* ptr, size_t size, size_t nmemb, void* datasource)
    {
        file::handle h = (file::handle)datasource;
        return file::read(h, ptr, (unsigned int)(size * nmemb));
    }

    int oxfile_ov_seek_func(void* datasource, ogg_int64_t offset, int whence)
    {
        file::handle h = (file::handle)datasource;
        return file::seek(h, (int)offset, whence);
    }

    int oxfile_ov_close_func(void* datasource)
    {
        file::handle h = (file::handle)datasource;
        file::close(h);
        return 0;
    }

    int oxfile_ov_noclose_func(void* datasource)
    {
        return 0;
    }

    long int oxfile_ov_tell_func(void* datasource)
    {
        file::handle h = (file::handle)datasource;
        return file::tell(h);
    }

    ov_callbacks cb_oxfile =
    {
        oxfile_ov_read_func,
        oxfile_ov_seek_func,
        oxfile_ov_close_func,
        oxfile_ov_tell_func
    };

    ov_callbacks cb_oxfileNotClose =
    {
        oxfile_ov_read_func,
        oxfile_ov_seek_func,
        oxfile_ov_noclose_func,
        oxfile_ov_tell_func
    };



    size_t file_ov_read_func(void* ptr, size_t size, size_t nmemb, void* datasource)
    {
        FILE* file = (FILE*)datasource;
        return fread(ptr, size, nmemb, file);
    }

    int file_ov_seek_func(void* datasource, ogg_int64_t offset, int whence)
    {
        FILE* file = (FILE*)datasource;
        return fseek(file, (int)offset, whence);
    }

    int file_ov_close_func(void* datasource)
    {
        FILE* file = (FILE*)datasource;
        fclose(file);
        return 0;
    }

    long int file_ov_tell_func(void* datasource)
    {
        FILE* file = (FILE*)datasource;
        return ftell(file);
    }

    ov_callbacks cb_fileStream =
    {
        file_ov_read_func,
        file_ov_seek_func,
        file_ov_close_func,
        file_ov_tell_func
    };



    OggStream::OggStream():
#if OXYGINE_VERSION < 6
        _memStream(0, 0), _section(0),  _info(0)
#else
        _memStream(0, 0, 0), _section(0), _info(0)
#endif
    {
        memset(&_vorbisFile, 0, sizeof(_vorbisFile));
    }

    OggStream::~OggStream()
    {
        release();
    }

    bool OggStream::init(const void* data, size_t len)
    {
        release();
        _memStream.init(data, len);

        return _init(cb_oxfileNotClose, &_memStream);
    }

    bool  OggStream::init(const char* name)
    {
        release();

#ifdef __S3E__
        _file = fopen(name, "rb");
        return _init(cb_fileStream, _file);
#else
        file::handle _oxfile = file::open(name, "srb");
        if (_oxfile)
            return _init(cb_oxfile, _oxfile);
        return false;
#endif
    }

    bool OggStream::init(file::handle h, bool close)
    {
        release();
        file::handle _oxfile = h;
        if (close)
            return _init(cb_oxfile, _oxfile);
        return _init(cb_oxfileNotClose, _oxfile);
    }

    bool OggStream::_init(const ov_callbacks& cb, void* userData)
    {
        _section = 0;
        _ended = true;

        if (ov_open_callbacks(userData, &_vorbisFile, 0, 0, cb) < 0)
            return false;

        _ended = false;

        char** ptr = ov_comment(&_vorbisFile, -1)->user_comments;
        if (ptr)
        {
        }
        _info = ov_info(&_vorbisFile, -1);

        _rate = _info->rate;
        _channels = _info->channels;
        _pcm = (unsigned int)ov_pcm_total(&_vorbisFile, -1);
        _duration = (int)(ov_time_total(&_vorbisFile, -1));

        return true;
    }

    int OggStream::getCurrentPCM() const
    {
        int r = (int)ov_pcm_tell(const_cast<OggVorbis_File*>(&_vorbisFile));
        return r;
    }

    int OggStream::getCurrentMS() const
    {
        int r = (int)ov_time_tell(const_cast<OggVorbis_File*>(&_vorbisFile));
        return r;
    }

    void OggStream::decodeAll(void* data, int bufferSize)
    {
        while (1)
        {
            int r = (int)ov_read(&_vorbisFile,
                                 (char*)data, bufferSize,
                                 &_section);

            bufferSize -= r;
            data = (char*)data + r;

            if (!r)
                break;
        }

        //OX_ASSERT(bufferSize == 0);
        _ended = true;
    }

    void OggStream::reset()
    {
        ov_pcm_seek(&_vorbisFile, 0);
        _ended = false;
    }

    void OggStream::release()
    {
        if (_vorbisFile.datasource)
            ov_clear(&_vorbisFile);

        _ended = true;
    }

    int OggStream::seekPCM(int pcm)
    {
        return ov_pcm_seek(&_vorbisFile, pcm);
    }

    int OggStream::seekMS(int ms)
    {
        return ov_time_seek(&_vorbisFile, ms);
    }

    int OggStream::decodeNextBlock(bool looped, void* data, int bufferSize)
    {
        unsigned int bytesUnpacked = 0;

        //int p = (int)ov_pcm_tell(&_vorbisFile);

        while (1)
        {
            int r = (int)ov_read(&_vorbisFile, (char*)data, bufferSize, &_section);
            if (!r)
            {
                if (looped)
                {
                    ov_pcm_seek(&_vorbisFile, 0);
                }
                else
                {
                    _ended = true;
                    break;
                }
            }

            data = (char*)data + r;
            bufferSize -= r;

            bytesUnpacked += r;
            if (bufferSize == 0)
                break;
        }

        return bytesUnpacked;
    }
}

