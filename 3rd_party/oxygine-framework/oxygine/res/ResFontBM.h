#pragma once
#include "../oxygine-include.h"
#include "ResFont.h"
#include "../core/Texture.h"
#include <vector>

namespace oxygine
{
    class ResFontBM: public ResFont
    {
    public:
        static Resource* create(CreateResourceContext& context);
        static Resource* createBM(CreateResourceContext& context);
        static Resource* createSD(CreateResourceContext& context);

        ResFontBM();
        ~ResFontBM();

        /**loads "fnt" font from file, supported XML */
        void init(QString fntPath, bool premultipliedAlpha = false);

        void cleanup();

        const Font* getFont(const char* name = nullptr, int size = 0) const override;
        const Font* getClosestFont(float worldScale, int styleFontSize, float& resScale) const override;

    private:
        struct page
        {
            QString file;
            spNativeTexture texture;
        };

        void addPage(int tw, int th, QString head, QString file);

        void _loadPage(const page& p, LoadResourcesContext*);
        void _load(LoadResourcesContext*) override;
        void _unload() override;
        void _restore(Restorable*, void*);
        void _createFont(CreateResourceContext* context, bool sd, bool bmc, int downsample);
        void _finalize();

        typedef std::vector<page> pages;
        pages _pages;
        Font* _font;
        bool _sdf;

        ImageData::TextureFormat _format;

        QString _file;
        bool _premultipliedAlpha;
    };
}
