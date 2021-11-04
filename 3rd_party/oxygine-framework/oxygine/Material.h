#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/core/ref_counter.h"
#include "3rd_party/oxygine-framework/oxygine/core/texture.h"
#include "3rd_party/oxygine-framework/oxygine/core/VideoDriver.h"
#include "3rd_party/oxygine-framework/oxygine/math/AffineTransform.h"

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
        explicit Material(const Material* other)
            : m_hash(other->m_hash),
              m_base(other->m_base),
              m_table(other->m_table),
              m_blend(other->m_blend),
              m_uberShader(other->m_uberShader),
              m_addColor(other->m_addColor),
              m_flags(other->m_flags)
        {
        }
        virtual ~Material() = default;
        void init();
        spMaterial clone() const
        {
            return spMaterial::create(this);
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
        void setMatrixMode(bool set);
    private:
        void xapply();
        void xflush();
    public:
        size_t m_hash{0};
        spTexture    m_base;
        spTexture    m_table;
        VideoDriver::blend_mode m_blend;
        UberShaderProgram* m_uberShader;
        QColor             m_addColor;
        qint32             m_flags;
    };    
}
