#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/core/Renderer.h"
#include "3rd_party/oxygine-framework/oxygine/core/ref_counter.h"
#include "3rd_party/oxygine-framework/oxygine/core/NativeTexture.h"

namespace oxygine
{
    class Material;
    using spMaterial = oxygine::intrusive_ptr<Material>;

    class Material : public ref_counter
    {
    public:
        static spMaterial current;
        static spMaterial null;

        explicit Material()
        {
            init();
        }
        Material(const Material& other);
        virtual ~Material() = default;
        void init();
        spMaterial clone() const
        {
            return spMaterial::create(*this);
        }
        void update(size_t &hash) const
        {
            hash = 0;
            rehash(hash);
        }
        bool compare(const Material* a, const Material* b);
        void rehash(size_t& hash) const;
        void apply();
        void flush();
        void render(const AffineTransform& tr, const QColor& c, const RectF& src, const RectF& dest);
        void render(const QColor& c, const RectF& src, const RectF& dest);
    private:
        void xapply();
        void xflush();
    public:
        size_t m_hash{0};
        spNativeTexture    m_base;
        spNativeTexture    m_table;
        spNativeTexture    m_alpha;
        blend_mode         m_blend;
        UberShaderProgram* m_uberShader;
        QColor             m_addColor;
        qint32             m_flags;
    };    
}
