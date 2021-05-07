#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-include.h"
#include "3rd_party/oxygine-framework/oxygine/core/Renderer.h"
#include "3rd_party/oxygine-framework/oxygine/core/ref_counter.h"

namespace oxygine
{
    class MaterialCache;

    class Material;
    using spMaterial = oxygine::intrusive_ptr<Material>;

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

        virtual void init() {}

        virtual void xapply() {}
        virtual void xflush() {}

        virtual spMaterial clone() const = 0;
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

    public:
        size_t m_hash;
        compare m_compare;
    };

    typedef intrusive_ptr<Material> spMaterialX;

    class NullMaterialX;
    typedef intrusive_ptr<NullMaterialX> spNullMaterialX;

    class NullMaterialX : public Material
    {
    public:
        NullMaterialX()
        {
            typedef bool (*fn)(const NullMaterialX&a, const NullMaterialX&b);
            fn f = &NullMaterialX::cmp;
            m_compare = (compare)f;
            init();
        }
        void copyTo(NullMaterialX &d) const{d = *this;}
        void copyFrom(const NullMaterialX &d) {*this = d;}
        spMaterial clone() const override {return spNullMaterialX::create(*this);}
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
    class STDMaterial : public Material
    {
    public:
        STDMaterial()
        {
            typedef bool (*fn)(const STDMaterial&a, const STDMaterial&b);
            fn f = &STDMaterial::cmp;
            m_compare = (compare)f;
            STDMaterial::init();
        }
        void copyTo(STDMaterial &d) const{d = *this;}
        void copyFrom(const STDMaterial &d) {*this = d;}
        spMaterial clone() const override {return spSTDMaterial::create(*this);}
        virtual void update(size_t &hash, compare &cm) const override
        {
            typedef bool (*fn)(const STDMaterial&a, const STDMaterial&b);
            fn f = &STDMaterial::cmp;\
            cm = (compare)f;\
            hash = 0;\
            rehash(hash);
        }

        static bool cmp(const STDMaterial& a, const STDMaterial& b);

        virtual void init() override;
        virtual void rehash(size_t& hash) const override;

        virtual void xapply() override;
        virtual void xflush() override;

        virtual void render(const AffineTransform& tr, const QColor& c, const RectF& src, const RectF& dest) override;
        virtual void render(const QColor& c, const RectF& src, const RectF& dest) override;

        spSTDMaterial cloneDefaultShader() const;

    public:
        spNativeTexture    m_base;
        spNativeTexture    m_table;
        spNativeTexture    m_alpha;
        blend_mode         m_blend;
        UberShaderProgram* m_uberShader;
        QColor             m_addColor;
        qint32             m_flags;
    };    
}
