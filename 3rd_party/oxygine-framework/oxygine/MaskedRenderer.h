#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/STDRenderer.h"

namespace oxygine
{
    class ClipUV
    {
    public:
        explicit ClipUV() = default;
        explicit ClipUV(const Vector2& a, const Vector2& b, const Vector2& c,
               const Vector2& a_uv, const Vector2& b_uv, const Vector2& c_uv) : m_uvA(a_uv)
        {
            m_dAB = b - a;
            m_dAC = c - a;

            float v = 1.0f / (m_dAB.x * m_dAC.y - m_dAB.y * m_dAC.x);
            m_dAB *= v;
            m_dAC *= v;

            m_dac.x = m_dAC.x * a.y - m_dAC.y * a.x;
            m_dac.y = -(m_dAB.x * a.y - m_dAB.y * a.x);

            m_dAB *= -1;

            m_dAB.x *= -1;
            m_dAC.x *= -1;

            m_duvAB = b_uv - m_uvA;
            m_duvAC = c_uv - m_uvA;
        }
        virtual ~ClipUV() = default;
        void get(VideoDriver::Uniform3f vec[4]) const
        {
            vec[0] = VideoDriver::Uniform3f(m_dac.x, m_dAC.y, m_dAC.x);
            vec[1] = VideoDriver::Uniform3f(m_dac.y, m_dAB.y, m_dAB.x);
            vec[2] = VideoDriver::Uniform3f(m_uvA.x, m_duvAB.x, m_duvAC.x);
            vec[3] = VideoDriver::Uniform3f(m_uvA.y, m_duvAB.y, m_duvAC.y);
        }

    protected:
        Vector2 m_dac;
        Vector2 m_uvA;

        Vector2 m_dAB;
        Vector2 m_dAC;
        Vector2 m_duvAB;
        Vector2 m_duvAC;
    };

    class MaskedRenderer : public STDRenderer
    {
    public:
        explicit MaskedRenderer(spTexture mask, const RectF& srcRect, const RectF& destRect, const AffineTransform& t, bool channelR, VideoDriver*);
        virtual ~MaskedRenderer() = default;
    protected:
        virtual void shaderProgramChanged() override;
    protected:
        VideoDriver::Uniform3f m_msk[4];
        VideoDriver::Uniform4f m_clipMask;
    };
}
