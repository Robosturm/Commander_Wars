#pragma once
#include "../oxygine-include.h"
#include "CreateResourceContext.h"
#include "ResAnim.h"
#include "Resource.h"

namespace oxygine
{
    class ResAtlas: public Resource
    {
    public:
        static Resource* create(CreateResourceContext& context);
        struct atlas
        {
            spNativeTexture base;
            QString base_path;

            spNativeTexture alpha;
            QString alpha_path;
        };


        ResAtlas();
        ~ResAtlas();

        void addAtlas(ImageData::TextureFormat tf, QString base, QString alpha, int w, int h);

        const atlas& getAtlas(int i) const {return _atlasses[i];}
        int          getNum() const { return (int)_atlasses.size(); }

    protected:
        void _restore(Restorable* r, void* user);

        void _load(LoadResourcesContext*) override;
        void _unload() override;

        //void loadAtlas(CreateResourceContext& context);
        ResAnim* createEmpty(const XmlWalker& walker, CreateResourceContext& context);
        static void init_resAnim(ResAnim* rs, QString file, QDomElement node);

    protected:
        //settings from xml
        bool _linearFilter;
        bool _clamp2edge;

        void loadBase(QDomElement node);

        std::vector<unsigned char> _hitTestBuffer;

        typedef std::vector<atlas> atlasses;
        atlasses _atlasses;
    };

    typedef void(*load_texture_hook)(QString file, spNativeTexture nt, bool linearFilter, bool clamp2edge, LoadResourcesContext* load_context);
    void set_load_texture_hook(load_texture_hook);
    void load_texture_internal(QString file, spNativeTexture nt, bool linearFilter, bool clamp2edge, LoadResourcesContext* load_context);
}
