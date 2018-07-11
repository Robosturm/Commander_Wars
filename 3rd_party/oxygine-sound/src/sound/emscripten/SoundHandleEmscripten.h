#pragma once
#include <string>
#include "../SoundHandle.h"

namespace oxygine
{
    class SoundSystemEmscripten;

    class SoundHandleEmscripten : public SoundHandle
    {
    public:
        SoundHandleEmscripten(const std::string& path);
        ~SoundHandleEmscripten();

    protected:
        friend class SoundSystemEmscripten;

        void _updateVolume() override;
        void _updatePitch() override;
        void _updateLoop() override;
        void _update() override;

        void _init() override;
        void _play() override;
        void _pause() override;
        void _resume() override;
        void _stop()  override;
        void _setPosition(int tm) override;

        timeMS _getPosition() const  override;

    private:
        std::string _path;
        int _handle;
    };
}