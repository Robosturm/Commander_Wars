#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-include.h"
#include "3rd_party/oxygine-framework/oxygine/STDRenderer.h"

namespace oxygine
{
    class ClipUV
    {
    public:
        ClipUV() {}
        ClipUV(const Vector2& a, const Vector2& b, const Vector2& c,
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

        void get(Vector3* vec) const
        {
            vec[0] = Vector3(m_dac.x, m_dAC.y, m_dAC.x);
            vec[1] = Vector3(m_dac.y, m_dAB.y, m_dAB.x);

            vec[2] = Vector3(m_uvA.x, m_duvAB.x, m_duvAC.x);
            vec[3] = Vector3(m_uvA.y, m_duvAB.y, m_duvAC.y);
        }

        Vector2 calc(const Vector2& pos) const
        {
            float a = Vector3(m_dac.x, m_dAC.y, m_dAC.x).dot(Vector3(1, pos.x, pos.y));
            float b = Vector3(m_dac.y, m_dAB.y, m_dAB.x).dot(Vector3(1, pos.x, pos.y));
            float u = Vector3(m_uvA.x, m_duvAB.x, m_duvAC.x).dot(Vector3(1, a, b));
            float v = Vector3(m_uvA.y, m_duvAB.y, m_duvAC.y).dot(Vector3(1, a, b));
            return Vector2(u, v);
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
        MaskedRenderer(spNativeTexture mask, const RectF& srcRect, const RectF& destRect, const Transform& t, bool channelR, IVideoDriver*);

    protected:
        void shaderProgramChanged() override;

        Vector3 m_msk[4];
        Vector4 m_clipMask;
    };
}
