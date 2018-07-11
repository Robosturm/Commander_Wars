#include "oxygine/core/file.h"
#include "oxygine/core/oxygine.h"
#include "oxygine/res/Resources.h"
#include "oxygine/res/CreateResourceContext.h"

#include "SoundPlayer.h"
#include "Sound.h"
#include "SoundInstance.h"
#include "SoundSystem.h"
#include "SoundHandle.h"

#include "ResSound.h"
#include "oxygine/actor/DebugActor.h"
#include <algorithm>

namespace oxygine
{
    void SoundPlayer::initialize()
    {
        Resources::registerResourceType(&ResSound::createResSound, "sound");
    }

    void SoundPlayer::free()
    {
        Resources::unregisterResourceType("sound");

    }

    SoundPlayer::SoundPlayer(): _resources(0), _volume(1.0f), _time(0), _lastUpdateTime(0), _paused(false)
    {
        _time = getTimeMS();
        _lastUpdateTime = _time;
    }

    SoundPlayer::~SoundPlayer()
    {

    }

    void SoundPlayer::setVolume(float v)
    {
        _volume = v;
        for (playingSounds::iterator i = _sounds.begin(); i != _sounds.end(); ++i)
        {
            SoundInstance* s = (*i).get();
            s->_updateVolume();
        }

        for (playingSounds::iterator i = _pausedSounds.begin(); i != _pausedSounds.end(); ++i)
        {
            SoundInstance* s = (*i).get();
            s->_updateVolume();
        }

    }

    void SoundPlayer::removeSoundInstance(SoundInstance* s)
    {
        playingSounds::iterator i = std::find(_sounds.begin(), _sounds.end(), s);
        //OX_ASSERT(i != _sounds.end());
        if (i != _sounds.end())
            _sounds.erase(i);
    }

    void SoundPlayer::addSoundInstance(SoundInstance* s)
    {
        playingSounds::iterator i = std::find(_sounds.begin(), _sounds.end(), s);
        //OX_ASSERT(i == _sounds.end());
        if (i == _sounds.end())
            _sounds.push_back(s);
    }

    spSoundInstance SoundPlayer::getSoundByIndex(int index)
    {
        return _sounds[index];
    }

    void SoundPlayer::setResources(Resources* res)
    {
        _resources = res;
    }


    spSoundInstance SoundPlayer::play(Sound* snd, const PlayOptions& opt)
    {
        return _play(snd, opt, "");
    }

    spSoundInstance SoundPlayer::_play(Sound* snd, const PlayOptions& opt, const string& name)
    {
        SoundHandle* handle = snd->createSH();
        if (!handle)
            return 0;

        spSoundInstance s = new SoundInstance(this, handle);

        s->setName(name);
        handle->setName(name);

        s->setPitch(opt._pitch);
        s->setLoop(opt._looped);
        if (opt._seek)
            s->seek(opt._seek);
        s->_updateVolume();

        if (opt._fadeIn)
            s->fadeIn(opt._fadeIn);
        else if (!opt._paused)
            s->play();


        return s;
    }


    spSoundInstance SoundPlayer::play(Resource* res, const PlayOptions& opt)
    {
        ResSound* ressound = safeCast<ResSound*>(res);
        if (!ressound || !ressound->getSound())
            return 0;

        spSoundInstance ins = _play(ressound->getSound(), opt, res->getName());
        return ins;
    }

    spSoundInstance SoundPlayer::play(const std::string& id, const PlayOptions& opt)
    {
        if (!_resources)
            return 0;

        ResSound* res = _resources->getT<ResSound>(id);
        if (!res)
            return 0;

        return play(res, opt);
    }

    void SoundPlayer::pause()
    {
        _pausedSounds.insert(_pausedSounds.end(), _sounds.begin(), _sounds.end());
        for (playingSounds::iterator i = _pausedSounds.begin(); i != _pausedSounds.end(); ++i)
        {
            SoundInstance* s = (*i).get();
            s->pause();
        }
        _paused = true;
    }

    void SoundPlayer::resume()
    {
        for (playingSounds::iterator i = _pausedSounds.begin(); i != _pausedSounds.end(); ++i)
        {
            SoundInstance* s = (*i).get();
            s->resume();
        }
        _pausedSounds.clear();
        _paused = false;
    }

    void SoundPlayer::stopByID(const string& id)
    {
        OX_ASSERT(0);
        /*
        bool try_again = true;
        while (try_again)
        {
            try_again = false;

            for (playingSounds::iterator i = _sounds.begin(); i != _sounds.end(); ++i)
            {
                spSoundInstance s = *i;
                if (!s->_channel)
                    continue;

                if (s->_desc.id == id)
                {
                    s->_channel->stop();
                }
            }
        }
        */
    }

    void SoundPlayer::stop()
    {
        while (!_sounds.empty())
            _sounds.back()->stop();
        /*
        for (playingSounds::iterator i = _sounds.begin(); i != _sounds.end(); ++i)
        {
            spSoundInstance sound = *i;
            sound->stop();
        }
        */

    }

    void SoundPlayer::fadeOut(int ms)
    {
        for (playingSounds::iterator i = _sounds.begin(); i != _sounds.end(); ++i)
        {
            spSoundInstance sound = *i;
            sound->fadeOut(ms);
        }
    }

    unsigned int SoundPlayer::getTime()const
    {
        return _time;
    }

    void SoundPlayer::update()
    {
        timeMS t = getTimeMS();
        if (!_paused)
            _time += t - _lastUpdateTime;


        for (size_t i = 0; i < _sounds.size();)
        {
            spSoundInstance s = _sounds[i];
            s->update();

            if (DebugActor::instance)
            {
                string name = s->getName();
                size_t p = name.find_last_of('/') + 1;
                name = name.substr(p);

                DebugActor::instance->addDebugString("%s '%s'", name.c_str(), state2str(s->getState()));
            }

            bool end = s->getState() == SoundInstance::Ended || s->getState() == SoundInstance::Paused;

            if (end)
            {
                _sounds.erase(_sounds.begin() + i);

                if (s->_cbDone)
                {
                    Event ev(Event::COMPLETE);
                    s->_cbDone(&ev);
                }
            }
            else
                ++i;
        }

        //logs::messageln("sounds %d", _sounds.size());

        _lastUpdateTime = t;
    }
}
