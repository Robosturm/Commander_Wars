#pragma once
#include <qopengl.h>

#include "3rd_party/oxygine-framework/oxygine/oxygine-include.h"
#include "3rd_party/oxygine-framework/oxygine/core/gl/VertexDeclarationGL.h"
#include "3rd_party/oxygine-framework/oxygine/core/VideoDriver.h"

namespace oxygine
{
    class VertexDeclarationGL;
    DECLARE_SMART(NativeTextureGLES, spNativeTextureGLES);

    /**intermediate gl driver with some shared gl code*/
    class VideoDriverGL: public IVideoDriver
    {
    public:
        VideoDriverGL();


        void    getViewport(Rect& r) const;
        bool    getScissorRect(Rect&) const;
        spNativeTexture getRenderTarget() const;
        const VertexDeclarationGL*  getVertexDeclaration(bvertex_format) const;

        void setScissorRect(const Rect*);
        void setRenderTarget(spNativeTexture rt);

        void setBlendFunc(BLEND_TYPE src, BLEND_TYPE dest);
        void setState(STATE, quint32 value);


    protected:
        quint32 getPT(IVideoDriver::PRIMITIVE_TYPE pt);
        quint32 getBT(IVideoDriver::BLEND_TYPE pt);
        void _begin(const Rect& viewport, const QColor* clearColor);

    protected:
        spNativeTextureGLES m_rt;

        mutable VertexDeclarations<VertexDeclarationGL> m_vdeclarations;
    };
}
