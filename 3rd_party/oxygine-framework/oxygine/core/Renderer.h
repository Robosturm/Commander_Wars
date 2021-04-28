#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-include.h"
#include "3rd_party/oxygine-framework/oxygine/core/NativeTexture.h"
#include "3rd_party/oxygine-framework/oxygine/core/VideoDriver.h"
#include "3rd_party/oxygine-framework/oxygine/math/AffineTransform.h"
#include "3rd_party/oxygine-framework/oxygine/math/Rect.h"

namespace oxygine
{
    typedef AffineTransform Transform;

    enum blend_mode
    {
        blend_disabled              = 0,
        blend_premultiplied_alpha   = ((IVideoDriver::BT_ONE                 << 16) | IVideoDriver::BT_ONE_MINUS_SRC_ALPHA),
        blend_alpha                 = ((IVideoDriver::BT_SRC_ALPHA           << 16) | IVideoDriver::BT_ONE_MINUS_SRC_ALPHA),
        blend_add                   = ((IVideoDriver::BT_ONE                 << 16) | IVideoDriver::BT_ONE),
        blend_screen                = ((IVideoDriver::BT_ONE                 << 16) | IVideoDriver::BT_ONE_MINUS_SRC_COLOR),
        blend_multiply              = ((IVideoDriver::BT_DST_COLOR           << 16) | IVideoDriver::BT_ONE_MINUS_SRC_ALPHA),
        blend_inverse               = ((IVideoDriver::BT_ONE_MINUS_DST_COLOR << 16) | IVideoDriver::BT_ZERO),
    };


    template<class V>
    void fillQuadT(V* pv, const RectF& srcRect, const RectF& destRect, const AffineTransform& transform, quint32 rgba)
    {
        float u = srcRect.pos.x;
        float v = srcRect.pos.y;

        float du = srcRect.size.x;
        float dv = srcRect.size.y;

        V vt;
        vt.color = rgba;

        const Vector2& pos = destRect.pos;
        const Vector2& size = destRect.size;

        Vector2 p1(pos.x, pos.y);
        Vector2 p2(pos.x, pos.y + size.y);
        Vector2 p3(pos.x + size.x, pos.y);
        Vector2 p4(pos.x + size.x, pos.y + size.y);


        p1 = transform.transform(p1);
        p2 = transform.transform(p2);
        p3 = transform.transform(p3);
        p4 = transform.transform(p4);

        vt.z = 0;

        vt.x = p1.x;
        vt.y = p1.y;
        vt.u = u;
        vt.v = v;
        *pv = vt;
        ++pv;

        vt.x = p2.x;
        vt.y = p2.y;
        vt.u = u;
        vt.v = v + dv;
        *pv = vt;
        ++pv;

        vt.x = p3.x;
        vt.y = p3.y;
        vt.u = u + du;
        vt.v = v;
        *pv = vt;
        ++pv;

        vt.x = p4.x;
        vt.y = p4.y;
        vt.u = u + du;
        vt.v = v + dv;
        *pv = vt;
        ++pv;
    }

    template<class V>
    void fillQuadZT(V* pv, const RectF& srcRect, const RectF& destRect, float Z, quint32 rgba)
    {
        float u = srcRect.pos.x;
        float v = srcRect.pos.y;

        float du = srcRect.size.x;
        float dv = srcRect.size.y;

        V vt;
        vt.color = rgba;

        const Vector2& pos = destRect.pos;
        const Vector2& size = destRect.size;

        Vector2 p1(pos.x, pos.y);
        Vector2 p2(pos.x, pos.y + size.y);
        Vector2 p3(pos.x + size.x, pos.y);
        Vector2 p4(pos.x + size.x, pos.y + size.y);


        vt.z = Z;

        vt.x = p1.x;
        vt.y = p1.y;
        vt.u = u;
        vt.v = v;
        *pv = vt;
        ++pv;

        vt.x = p2.x;
        vt.y = p2.y;
        vt.u = u;
        vt.v = v + dv;
        *pv = vt;
        ++pv;

        vt.x = p3.x;
        vt.y = p3.y;
        vt.u = u + du;
        vt.v = v;
        *pv = vt;
        ++pv;

        vt.x = p4.x;
        vt.y = p4.y;
        vt.u = u + du;
        vt.v = v + dv;
        *pv = vt;
        ++pv;
    }


    template<class V>
    void fillQuadT2(V* pv, const RectF& srcRect, const RectF& srcRect2, const RectF& destRect, const AffineTransform& transform, quint32 rgba)
    {
        float u = srcRect.pos.x;
        float v = srcRect.pos.y;

        float du = srcRect.size.x;
        float dv = srcRect.size.y;

        float u2 = srcRect2.pos.x;
        float v2 = srcRect2.pos.y;

        float du2 = srcRect2.size.x;
        float dv2 = srcRect2.size.y;

        V vt;
        vt.color = rgba;

        const Vector2& pos = destRect.pos;
        const Vector2& size = destRect.size;

        Vector2 p1(pos.x, pos.y);
        Vector2 p2(pos.x, pos.y + size.y);
        Vector2 p3(pos.x + size.x, pos.y);
        Vector2 p4(pos.x + size.x, pos.y + size.y);


        p1 = transform.transform(p1);
        p2 = transform.transform(p2);
        p3 = transform.transform(p3);
        p4 = transform.transform(p4);

        vt.z = 0;

        vt.x = p1.x;
        vt.y = p1.y;
        vt.u = u;
        vt.v = v;
        vt.u2 = u2;
        vt.v2 = v2;
        *pv = vt;
        ++pv;

        vt.x = p2.x;
        vt.y = p2.y;
        vt.u = u;
        vt.v = v + dv;
        vt.u2 = u2;
        vt.v2 = v2 + dv2;
        *pv = vt;
        ++pv;

        vt.x = p3.x;
        vt.y = p3.y;
        vt.u = u + du;
        vt.v = v;
        vt.u2 = u2 + du2;
        vt.v2 = v2;
        *pv = vt;
        ++pv;

        vt.x = p4.x;
        vt.y = p4.y;
        vt.u = u + du;
        vt.v = v + dv;
        vt.u2 = u2 + du2;
        vt.v2 = v2 + dv2;
        *pv = vt;
        ++pv;
    }

    class Material;
    typedef intrusive_ptr<Material> spMaterialX;

    /**Returns View matrix where Left Top corner is (0,0), and right bottom is (w,h)*/
    Matrix makeViewMatrix(qint32 w, qint32 h, bool flipU = false);
}
