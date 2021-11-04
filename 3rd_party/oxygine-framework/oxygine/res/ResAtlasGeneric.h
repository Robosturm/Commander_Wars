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
        explicit ResAtlasGeneric() = default;
        virtual ~ResAtlasGeneric() = default;
        void loadAtlas(CreateResourceContext& context);
    protected:
        virtual void _load() override;
        virtual void _unload() override;

    private:
        qint32 m_current{0};
    };
}
