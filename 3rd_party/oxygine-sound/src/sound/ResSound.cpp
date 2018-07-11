#include "ResSound.h"
#include "oxygine/core/file.h"
#include "Sound.h"
#include "SoundSystem.h"
#include "oxygine/res/CreateResourceContext.h"
#include "oxygine/res/Resources.h"
#include "oxygine/utils/stringUtils.h"

namespace oxygine
{
    Resource* ResSound::createResSound(CreateResourceContext& context)
    {
        ResSound* rs = new ResSound;
        rs->init(context);
        setNode(rs, context.walker.getNode());
        context.resources->add(rs);

        return rs;
    }

    ResSound* ResSound::create(const std::string& file, bool streaming)
    {
        ResSound* rs = new ResSound;
        rs->init(file, streaming);
        rs->setName(file);
        return rs;
    }

    ResSound* ResSound::create(std::vector<unsigned char>& data, bool swap)
    {
        ResSound* rs = new ResSound;
        rs->init(data, swap);
        return rs;
    }

    ResSound* ResSound::create(const std::vector<unsigned char>& data_)
    {
        std::vector<unsigned char>& data = const_cast<std::vector<unsigned char>& >(data_);
        return create(data, false);
    }

    ResSound::ResSound() : _sound(0), _streaming(false)
    {

    }

    ResSound::~ResSound()
    {
        delete _sound;
    }

    bool ResSound::init(const std::string& file, bool streaming)
    {
        _streaming = streaming;
        char str[512];
        path::normalize(file.c_str(), str);
        _file = str;
        return true;
    }

    bool ResSound::init(std::vector<unsigned char>& data, bool swap)
    {
        _streaming = false;
        _sound = SoundSystem::get()->createSound(data, swap);
        if (_sound)
            _sound->setRes(this);

        return _sound != 0;
    }

    bool ResSound::init(CreateResourceContext& context)
    {
        std::string file = context.walker.getNode().attribute("file").as_string();
        _streaming = context.walker.getNode().attribute("streaming").as_bool(_streaming);

#if EMSCRIPTEN
        _streaming = true;
#endif

        setName(Resource::extractID(context.walker.getNode(), file, ""));
        _file = context.walker.getCurrentFolder() + file;

        char nfile[512];
        path::normalize(_file.c_str(), nfile);
        _file = nfile;

        return true;
    }

    const std::string&  ResSound::getPath() const
    {
        return _file;
    }

    timeMS  ResSound::getDuration() const
    {
        return getSound()->getDuration();
    }


    Sound* ResSound::_getSound()
    {
        if (!_sound)
        {
            _sound = SoundSystem::get()->createSound(_file.c_str(), _streaming);

            if (_sound)
                _sound->setRes(this);
        }

        return _sound;
    }

    Sound* ResSound::getSound() const
    {
        return const_cast<ResSound*>(this)->_getSound();
    }

    void ResSound::_load(LoadResourcesContext* context)
    {
        _getSound();
    }

    void ResSound::_unload()
    {
        if (!_sound)
            return;

    }

    void emscStartSoundsPreloading(Resources& resources)
    {
#ifdef EMSCRIPTEN
        Resources::resources lst;
        resources.collect(lst);
        for (size_t i = 0; i < lst.size(); ++i)
        {
            ResSound* rs = dynamic_cast<ResSound*>(lst[i].get());
            if (!rs)
                continue;

            EM_ASM_ARGS(
            {
                preload_file(Pointer_stringify($0));
            }, rs->getPath().c_str());
        }
#endif
    }

    int emscGetNumPreloadingSounds()
    {
#ifdef EMSCRIPTEN

        int r = EM_ASM_ARGS(
        {
            return assetsPreloader.getLeft();
        }, 0);
        return r;
#endif
        return 0;
    }
}
