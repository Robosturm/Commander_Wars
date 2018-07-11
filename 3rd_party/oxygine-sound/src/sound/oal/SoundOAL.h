#pragma once
#include <vector>
#include <string>
#include "../OggStream.h"
#include "../Sound.h"
#include "../oal.h"
#include "SoundHandleOAL.h"

namespace oxygine
{
    class WavStream;
    class OggStream;
    class SoundHandleOAL;

    class SoundOAL: public Sound
    {
    public:
        enum Type
        {
            Unknown, Buffer, Ogg, Wav
        };

        SoundOAL(const std::string& path, file::handle fh);
        SoundOAL(std::vector<unsigned char>& data, bool swap);
        ~SoundOAL();

        unsigned int    getAlBuffer() const {return _alBuffer;}
        int             getDuration() const override;
        unsigned int    getFormat() const {return _format;}
        Type            getType() const { return _type; }

        void initStream(OggStream& stream);
        void initStream(WavStream& stream);

        SoundHandleOAL* createSH() override;

    private:
        bool _init(SoundStream& stream);

        SoundStream* init0(char header[4], OggStream& oggStream, WavStream& wavStream);

        unsigned int _format;
        int _timeMS;

        std::string _fileName;

        Type _type;

        ALuint _alBuffer;
        std::vector<unsigned char> _fileBuffer;
    };
}
