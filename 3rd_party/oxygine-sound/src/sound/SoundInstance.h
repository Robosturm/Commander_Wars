#pragma once
#include "oxygine/core/Object.h"
#include "oxygine/EventDispatcher.h"
#include "oxygine/Event.h"

namespace oxygine
{
    class SoundPlayer;
    class SoundHandle;


    DECLARE_SMART(SoundInstance, spSoundInstance);
    class SoundInstance: public Object
    {
    public:
        class SoundEvent: public Event
        {
        public:
            enum
            {
                EVENT_ABOUT_DONE = makefourcc('S', 'I', 'A', 'D'),
                EVENT_DONE = Event::COMPLETE,
            };
            SoundEvent(eventType tp, SoundInstance* ins): Event(tp), instance(ins) {}
            SoundInstance* instance;
        };
        SoundInstance(SoundPlayer* player, SoundHandle*);
        ~SoundInstance();

        enum State
        {
            FadingIn,
            FadingOutStop,
            FadingOutPause,
            Normal,
            Paused,
            Stopped,
            Ended,
        };

        void play();
        void pause();
        void resume();

        /**SoundInstance shouldn't be used after "stop"*/
        void stop();

        /**could be used after fadeOut with stop=false*/
        void fadeIn(int fadeInMS);

        void fadeOut(int fadeOutMS);
        /**Event::COMPLETE won't be dispatched*/
        void fadeOutPause(int fadeOutMS);



        int         getDuration() const;
        int         getPosition() const;
        float       getVolume() const;
        float       getCurrentVolume() const;
        float       getPitch() const;
        State       getState() const { return _state; }

        bool        isLooped() const;
        bool        isPlaying() const;
        bool        isPaused() const;
        bool        isFadingOut() const;
        bool        isFadedOut() const;

        void    setVolume(float v);
        void    setCoord(const Vector2& pos, float z = 0);
        void    setPitch(float v);
        void    setLoop(bool loop);
        void    seek(int tm);

        /**called when sound done*/
        void    setDoneCallback(EventCallback cb) {_cbDone = cb;}
        void    setAboutDoneCallback(EventCallback cb) {_cbAboutDone = cb;}

    protected:

        friend class SoundPlayer;
        void finished();
        void update();
        void aboutDone();

        void _fadeIn(int dur);
        void _fadeOut(int dur, bool stop);
        void _updateVolume();
        void _setHanleVolume(float v);


        SoundHandle* _handle;

    private:
        SoundPlayer* _player;
        EventCallback _cbDone;
        EventCallback _cbAboutDone;

        bool _finished;
        bool _fadedOut;

        float _volume;//primary volume
        unsigned int _startTime;

        int _startFadeIn;
        int _startFadeOut;

        int _fadeInMS;
        int _fadeOutMS;


        State _state;
    };


    const char* state2str(SoundInstance::State s);
}