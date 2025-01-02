#pragma once
#include <QTransform>
#include <QRect>

#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "texture.h"
#include "3rd_party/oxygine-framework/oxygine/core/VideoDriver.h"
#include "3rd_party/oxygine-framework/oxygine/core/UberShaderProgram.h"

#include "coreengine/memorymanagement.h"
#include "coreengine/refobject.h"

namespace oxygine
{
class Material;
using spMaterial = std::shared_ptr<Material>;

class Material final : public RefObject<Material>
{
public:
    static constexpr const char* const getTypeName()
    {
        return "Material";
    }
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
        m_addColor(other->m_addColor),
        m_fracShader(other->m_fracShader)
    {
    }
    virtual ~Material() = default;
    void init();
    spMaterial clone() const
    {
        return MemoryManagement::create<Material>(this);
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
    void render(const QTransform& tr, const QColor& c, const QRectF& src, QRect& dest);
    void setMatrixMode(bool set);
private:
    void xapply();
    void xflush();
public:
    size_t m_hash{0};
    spTexture    m_base;
    spTexture    m_table;
    VideoDriver::BLEND_MODE m_blend;
    QColor             m_addColor;
    UberShaderProgram::ColorMode m_fracShader;
};
}
