#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-include.h"
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
        struct atlas
        {
            spNativeTexture base;
            QString base_path;

            spNativeTexture alpha;
            QString alpha_path;
        };

        ResAtlas();
        ~ResAtlas();

        void addAtlas(ImageData::TextureFormat tf, QString base, QString alpha, qint32 w, qint32 h);

        const atlas& getAtlas(qint32 i) const {return m_atlasses[i];}
        qint32          getNum() const { return (int)m_atlasses.size(); }

        virtual void setLinearFilter(quint32 linearFilter) override;
        virtual quint32 getLinearFilter() const override;

    protected:
        void _restore(Restorable* r);

        void _load(LoadResourcesContext*) override;
        void _unload() override;

        //void loadAtlas(CreateResourceContext& context);
        spResAnim createEmpty(const XmlWalker& walker, CreateResourceContext& context);
        static void init_resAnim(spResAnim rs, QString file, QDomElement node);
        void loadBase(QDomElement node);

    protected:
        //settings from xml
        quint32 m_linearFilter;
        bool m_clamp2edge;
        QVector<unsigned char> m_hitTestBuffer;

        typedef QVector<atlas> atlasses;
        atlasses m_atlasses;
    };

    typedef void(*load_texture_hook)(QString file, spNativeTexture nt, quint32 linearFilter, bool clamp2edge, LoadResourcesContext* load_context);
    void load_texture_internal(QString file, spNativeTexture nt, quint32 linearFilter, bool clamp2edge, LoadResourcesContext* load_context);
}
