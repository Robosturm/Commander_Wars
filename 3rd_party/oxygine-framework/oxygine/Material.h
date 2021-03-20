#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-include.h"
#include "3rd_party/oxygine-framework/oxygine/core/Renderer.h"
#include "3rd_party/oxygine-framework/oxygine/core/ref_counter.h"

namespace oxygine
{
    class MaterialCache;

    class Material : public ref_counter
    {
    public:

        static spMaterialX current;
        static spMaterialX null;

        typedef bool(*compare)(const Material* a, const Material* b);

        Material();

        Material& operator = (const Material& r);
        Material(compare cmp);
        Material(const Material& other);


        size_t _hash;
        compare _compare;

        virtual void init() {}

        virtual void xapply() {}
        virtual void xflush() {}

        virtual Material* clone() const = 0;
        virtual void update(size_t& hash, compare&) const = 0;
        virtual void rehash(size_t& hash) const = 0;

        virtual void render(const AffineTransform& , const QColor& , const RectF& , const RectF& ) {}
        virtual void render(const QColor& , const RectF& , const RectF& ) {}

        void apply();
        void flush();


        template <class T>
        void apply2(const T& f)
        {
            apply();
            f();
        }
    };

    typedef intrusive_ptr<Material> spMaterialX;


    class NullMaterialX : public Material
    {
    public:
        NullMaterialX()
        {
            typedef bool (*fn)(const NullMaterialX&a, const NullMaterialX&b);
            fn f = &NullMaterialX::cmp;
            _compare = (compare)f;
            init();
        }
        void copyTo(NullMaterialX &d) const{d = *this;}
        void copyFrom(const NullMaterialX &d) {*this = d;}
        NullMaterialX* clone() const override {return new NullMaterialX(*this);}
        virtual void update(size_t &hash, compare &cm) const override
        {
            typedef bool (*fn)(const NullMaterialX&a, const NullMaterialX&b);
            fn f = &NullMaterialX::cmp;\
            cm = (compare)f;\
            hash = 0;\
            rehash(hash);
        }
        static bool cmp(const NullMaterialX&, const NullMaterialX&) { return false; }
        virtual void rehash(size_t&) const override {}
    };

    DECLARE_SMART(STDMaterial, spSTDMaterial);
    class STDMaterial: public Material
    {
    public:
        STDMaterial()
        {
            typedef bool (*fn)(const STDMaterial&a, const STDMaterial&b);
            fn f = &STDMaterial::cmp;
            _compare = (compare)f;
            init();
        }
        void copyTo(STDMaterial &d) const{d = *this;}
        void copyFrom(const STDMaterial &d) {*this = d;}
        STDMaterial* clone() const override {return new STDMaterial(*this);}
        virtual void update(size_t &hash, compare &cm) const override
        {
            typedef bool (*fn)(const STDMaterial&a, const STDMaterial&b);
            fn f = &STDMaterial::cmp;\
            cm = (compare)f;\
            hash = 0;\
            rehash(hash);
        }

        spNativeTexture    _base;
        spNativeTexture    _table;
        spNativeTexture    _alpha;
        blend_mode         _blend;
        UberShaderProgram* _uberShader;
        QColor              _addColor;
        int                _flags;

        static bool cmp(const STDMaterial& a, const STDMaterial& b);

        virtual void init() override;
        virtual void rehash(size_t& hash) const override;

        virtual void xapply() override;
        virtual void xflush() override;

        virtual void render(const AffineTransform& tr, const QColor& c, const RectF& src, const RectF& dest) override;
        virtual void render(const QColor& c, const RectF& src, const RectF& dest) override;

        spSTDMaterial cloneDefaultShader() const;
    };
    
}
