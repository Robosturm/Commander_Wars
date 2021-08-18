#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
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

        explicit ResFontBM() = default;
        virtual ~ResFontBM();

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
            spTexture texture;
        };
        void addPage(qint32 tw, qint32 th, QString head, QString file);
        void _loadPage(const page& p);
        virtual void _load() override;
        virtual void _unload() override;
        void _createFont(CreateResourceContext* context);
        void _finalize();

    private:
        using pages = QVector<page>;
        pages m_pages;
        spFont m_font;
        QString m_file;
        bool m_premultipliedAlpha{false};
        //settings from xml
        quint32 m_linearFilter{0};
        bool m_clamp2edge{false};

    };
}
