#include "3rd_party/oxygine-framework/oxygine/MaskedRenderer.h"
#include "3rd_party/oxygine-framework/oxygine/RenderState.h"
#include "3rd_party/oxygine-framework/oxygine/core/UberShaderProgram.h"
#include "3rd_party/oxygine-framework/oxygine/core/VertexDeclaration.h"

namespace oxygine
{
    MaskedRenderer::MaskedRenderer(spNativeTexture mask,
                                   const RectF& srcRect, const RectF& destRect,
                                   const Transform& t, bool channelR, IVideoDriver* driver) : STDRenderer(driver)
    {
        ClipUV clipUV = ClipUV(
                            t.transform(destRect.getLeftTop()),
                            t.transform(destRect.getRightTop()),
                            t.transform(destRect.getLeftBottom()),
                            srcRect.getLeftTop(),
                            srcRect.getRightTop(),
                            srcRect.getLeftBottom());

        RectF clipMask = srcRect;
        Vector2 v(1.0f / mask->getWidth(), 1.0f / mask->getHeight());
        clipMask.expand(v, v);

        m_baseShaderFlags |= UberShaderProgram::MASK;
        if (channelR)
        {
            m_baseShaderFlags |= UberShaderProgram::MASK_R_CHANNEL;
        }

        clipUV.get(m_msk);
        m_clipMask = Vector4(clipMask.getLeft(), clipMask.getTop(), clipMask.getRight(), clipMask.getBottom());

        rsCache().setTexture(UberShaderProgram::SAMPLER_MASK, mask);
    }

    void MaskedRenderer::shaderProgramChanged()
    {
        m_driver->setUniform("clip_mask", m_clipMask);
        m_driver->setUniform("msk", m_msk, 4);
    }
}
