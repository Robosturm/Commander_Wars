#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/res/CreateResourceContext.h"
#include "3rd_party/oxygine-framework/oxygine/res/ResAnim.h"
#include "3rd_party/oxygine-framework/oxygine/res/Resource.h"

namespace oxygine
{
    class ResAtlas;
    using spResAtlas = intrusive_ptr<ResAtlas>;

    class ResAtlas: public Resource
    {
    public:
        static spResource create(CreateResourceContext& context);
        explicit ResAtlas() = default;
        virtual ~ResAtlas() = default;
    protected:
        virtual void _load() = 0;
        virtual void _unload() = 0;
        spResAnim createEmpty(const XmlWalker& walker, CreateResourceContext& context);
        static void init_resAnim(spResAnim & rs, const QString & file, const QDomElement & node);
        void loadBase(QDomElement node);
    protected:
        //settings from xml
        quint32 m_linearFilter{GL_NEAREST};
        bool m_clamp2edge{false};
    };
}
