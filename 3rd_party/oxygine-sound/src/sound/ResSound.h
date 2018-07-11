#pragma once
#include "oxygine/res/Resource.h"

namespace oxygine
{
    class Sound;
    class SoundSystem;
    class CreateResourceContext;

    class ResSound: public Resource
    {
    public:
        static Resource* createResSound(CreateResourceContext& context);
        static ResSound* create(const std::string& file, bool streaming);
        static ResSound* create(std::vector<unsigned char>& data, bool swap);
        static ResSound* create(const std::vector<unsigned char>& data);

        ResSound();
        ~ResSound();

        bool init(const std::string& file, bool streaming);
        bool init(std::vector<unsigned char>& data, bool swap);
        bool init(CreateResourceContext& context);

        Sound*              getSound() const;
        const std::string&  getPath() const;
        timeMS              getDuration() const;

    private:
        Sound* _getSound();

        void _load(LoadResourcesContext* context) override;
        void _unload() override;

        std::string _file;
        bool _streaming;

        Sound* _sound;
    };


    void emscStartSoundsPreloading(Resources& res);
    int emscGetNumPreloadingSounds();
}