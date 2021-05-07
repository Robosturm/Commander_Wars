#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-include.h"
#include "3rd_party/oxygine-framework/oxygine/res/ResFont.h"
#include "3rd_party/oxygine-framework/oxygine/core/Texture.h"
#include "3rd_party/oxygine-framework/oxygine/Font.h"

namespace oxygine
{
    class ResFontBM;
    using spResFontBM = intrusive_ptr<ResFontBM>;

    class ResFontBM: public ResFont
    {
    public:
        static spResource create(CreateResourceContext& context);
        static spResource createBM(CreateResourceContext& context);
        static spResource createSD(CreateResourceContext& context);

        ResFontBM();
        ~ResFontBM();

        /**loads "fnt" font from file, supported XML */
        void init(QString fntPath, bool premultipliedAlpha = false);

        void cleanup();

        const Font* getFont(QString name = "", qint32 size = 0) const override;
        const Font* getClosestFont(float worldScale, qint32 styleFontSize, float& resScale) const override;
    private:
        void loadBase(QDomElement node);
    private:
        struct page
        {
            QString file;
            spNativeTexture texture;
        };
        void addPage(qint32 tw, qint32 th, QString head, QString file);
        void _loadPage(const page& p, LoadResourcesContext*);
        void _load(LoadResourcesContext*) override;
        void _unload() override;
        void _restore(Restorable*);
        void _createFont(CreateResourceContext* context, bool sd, bool bmc, qint32 downsample);
        void _finalize();

    private:
        typedef QVector<page> pages;
        pages m_pages;
        spFont m_font;
        bool m_sdf;
        QString m_file;
        bool m_premultipliedAlpha;

        //settings from xml
        quint32 m_linearFilter;
        bool m_clamp2edge;
    };
}
