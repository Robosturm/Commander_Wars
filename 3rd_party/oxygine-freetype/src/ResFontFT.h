#pragma once
#include "oxygine/res/ResFont.h"
#include "oxygine/utils/AtlasBuilder.h"
#include "oxygine/core/file.h"
#include <list>

extern "C"
{
    typedef struct FT_FaceRec_*  FT_Face;
}

namespace oxygine
{
    class CreateResourceContext;
    class FontFT;
    class Font;
    class Image;
    struct glyph;
    typedef size_t glyphOptions;

    class ResFontFT : public ResFont
    {
    public:
        static void initLibrary();
        static void freeLibrary();
        static int  getSnapSize();

        static void setSnapSize(int size);
        static void setMaxSnapSize(int size);
        static void setAtlasSize(int w, int h);
        static void setGlobalWorldScale(float s);

        struct postProcessData
        {
            //you could modify src pixels too
            ImageData* src;

            //where you should write result
            Image* dest;

            //your value passed to TextStyle/TextField
            glyphOptions opt;

            glyph* gl;
            const Font* font;
        };
        typedef void(*postProcessHook)(postProcessData&);

        static void setGlyphPostProcessor(postProcessHook);

        ResFontFT();
        ~ResFontFT();

        void init(const std::string& fnt);

        const Font* getFont(const char* name, int size) const override;
        const Font* getClosestFont(float worldScale, int styleFontSize, float& resScale) const override;

    protected:
        friend class FontFT;

        static Resource* createResource(CreateResourceContext& context);
        void _load(LoadResourcesContext* context) override;
        void _unload() override;

        Font* getFont(int size);

        MultiAtlas _atlas;
        spTexture createTexture(int w, int h);

        FT_Face _face;

        typedef std::list<FontFT> fonts;
        fonts _fonts;
        file::buffer _fdata;
    };



    void ftGenDefault(ResFontFT::postProcessData& data);
}
