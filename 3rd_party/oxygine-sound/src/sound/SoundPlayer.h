#pragma once
#include <string>
#include <vector>
#include "oxygine/core/ref_counter.h"
#include "oxygine/core/oxygine.h"

namespace oxygine
{
    class Resource;
    class ResSound;
    class Resources;
    class Sound;
    struct sound_desc;

    using namespace std;


    class PlayOptions
    {
    public:
        PlayOptions() : _looped(false), _pitch(1.0f), _fadeIn(0), _fadeOut(0), _paused(false), _volume(-1.0f), _seek(0) {}

        PlayOptions& loop(bool loop = true) { _looped = loop; return *this; }
        PlayOptions& pitch(float v)  { _pitch = v; return *this; }
        PlayOptions& volume(float v)  { _volume = v; return *this; }
        PlayOptions& fade(timeMS fadeIn, timeMS fadeOut = 0) { _fadeIn = fadeIn; _fadeOut = fadeOut; return *this; }
        PlayOptions& pause() { _paused = true; return *this; }
        PlayOptions& seek(timeMS ms) { _seek = ms; return *this; }


        float _pitch;
        float _volume;
        timeMS _fadeIn;
        timeMS _fadeOut;
        bool _paused;
        bool _looped;
        timeMS _seek;
    };



    DECLARE_SMART(SoundInstance, spSoundInstance);
    class SoundHandle;


    class SoundPlayer
    {
    public:
        static void initialize();
        static void free();

        SoundPlayer();
        ~SoundPlayer();


        spSoundInstance play(Resource*, const PlayOptions& = PlayOptions());
        spSoundInstance play(Sound*, const PlayOptions& = PlayOptions());
        spSoundInstance play(const std::string& id, const PlayOptions& = PlayOptions());

        spSoundInstance getSoundByIndex(int index);
        int             getSoundsNum()const {return (int)_sounds.size();}
        unsigned int    getTime() const;
        float           getVolume() const { return _volume; }

        void pause();
        void resume();

        void stopByID(const string& id);
        void stop();

        void fadeOut(int ms);

        void update();

        void setVolume(float v);
        void setResources(Resources* res);

        bool IsPaused() { return _paused; }


    private:
        friend class SoundInstance;
        void removeSoundInstance(SoundInstance*);
        void addSoundInstance(SoundInstance*);

        spSoundInstance _play(Sound*, const PlayOptions& opt, const string& name);


        Resources* _resources;
        float _volume;

        typedef std::vector<spSoundInstance> playingSounds;
        playingSounds _sounds;

        playingSounds _pausedSounds;

        timeMS _time;
        timeMS _lastUpdateTime;

        bool _paused;
    };

    class SoundPlayer2
    {
    public:

    };

}