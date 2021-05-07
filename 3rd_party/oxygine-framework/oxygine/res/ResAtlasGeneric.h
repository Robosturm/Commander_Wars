#pragma once
#include "3rd_party/oxygine-framework/oxygine/res/ResAtlas.h"

namespace oxygine
{

    struct atlas_data;

    class ResAtlasGeneric;
    using spResAtlasGeneric = intrusive_ptr<ResAtlasGeneric>;

    class ResAtlasGeneric : public ResAtlas
    {
    public:
        void nextAtlas(qint32 w, qint32 h, ImageData::TextureFormat tf, atlas_data& ad);
        void applyAtlas(atlas_data& ad, quint32 filter, bool clamp2edge);

        void loadAtlas(CreateResourceContext& context);
        void loadAtlas2(CreateResourceContext& context);

        void _load(LoadResourcesContext*) override;
        void _unload() override;

    private:
        qint32 m_current;
    };
}
