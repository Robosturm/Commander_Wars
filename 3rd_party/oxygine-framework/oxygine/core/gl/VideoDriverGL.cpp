#include "3rd_party/oxygine-framework/oxygine/core/gl/VideoDriverGL.h"
#include "3rd_party/oxygine-framework/oxygine/core/gl/NativeTextureGLES.h"
#include "3rd_party/oxygine-framework/oxygine/core/gamewindow.h"

namespace oxygine
{
    VideoDriverGL::VideoDriverGL()
    {
        m_rt = spNativeTextureGLES::create();
    }

    quint32 VideoDriverGL::getPT(IVideoDriver::PRIMITIVE_TYPE pt)
    {
        switch (pt)
        {
            case PT_POINTS:
                return GL_POINTS;
            case PT_LINES:
                return GL_LINES;
            case PT_LINE_LOOP:
                return GL_LINE_LOOP;
            case PT_LINE_STRIP:
                return PT_LINE_STRIP;
            case PT_TRIANGLES:
                return GL_TRIANGLES;
            case PT_TRIANGLE_STRIP:
                return GL_TRIANGLE_STRIP;
            case PT_TRIANGLE_FAN:
                return GL_TRIANGLE_FAN;
            default:
                break;
        }
        oxygine::handleErrorPolicy(oxygine::ep_show_error, "VideoDriverGL::getPT unknown primitive type");
        return PT_POINTS;
    }

    quint32 VideoDriverGL::getBT(IVideoDriver::BLEND_TYPE pt)
    {
        switch (pt)
        {
            case BT_ZERO:
                return GL_ZERO;
            case BT_ONE:
                return GL_ONE;
            case BT_SRC_COLOR:
                return GL_SRC_COLOR;
            case BT_ONE_MINUS_SRC_COLOR:
                return GL_ONE_MINUS_SRC_COLOR;
            case BT_SRC_ALPHA:
                return GL_SRC_ALPHA;
            case BT_ONE_MINUS_SRC_ALPHA:
                return GL_ONE_MINUS_SRC_ALPHA;
            case BT_DST_COLOR:
                return GL_DST_COLOR;
            case BT_DST_ALPHA:
                return GL_DST_ALPHA;
            case BT_ONE_MINUS_DST_ALPHA:
                return GL_ONE_MINUS_DST_ALPHA;
            case BT_ONE_MINUS_DST_COLOR:
                return GL_ONE_MINUS_DST_COLOR;
        }
        oxygine::handleErrorPolicy(oxygine::ep_show_error, "VideoDriverGL::getBT unknown blend");
        return GL_ONE;
    }

    spNativeTexture VideoDriverGL::getRenderTarget() const
    {
        return m_rt;
    }

    const VertexDeclarationGL* VideoDriverGL::getVertexDeclaration(bvertex_format fmt) const
    {
        return m_vdeclarations.get(fmt);
    }

    void VideoDriverGL::getViewport(Rect& r) const
    {
        GLint vp[4];
        GameWindow* window = oxygine::GameWindow::getWindow();
        window->glGetIntegerv(GL_VIEWPORT, vp);
        qreal ratio = window->devicePixelRatio();
        r = Rect(vp[0] / ratio, vp[1] / ratio,
                 vp[2] / ratio, vp[3] / ratio);
    }

    void VideoDriverGL::setScissorRect(const Rect* rect)
    {
        GameWindow* window = oxygine::GameWindow::getWindow();
        if (rect)
        {
            qreal ratio = window->devicePixelRatio();
            window->glEnable(GL_SCISSOR_TEST);
            window->glScissor(rect->getX() * ratio, rect->getY() * ratio,
                              rect->getWidth() * ratio, rect->getHeight() * ratio);
        }
        else
        {
            window->glDisable(GL_SCISSOR_TEST);
        }
    }

    bool VideoDriverGL::getScissorRect(Rect& r) const
    {
        GameWindow* window = oxygine::GameWindow::getWindow();
        GLboolean scrTest = window->glIsEnabled(GL_SCISSOR_TEST);

        GLint box[4];
        qreal ratio = window->devicePixelRatio();
        window->glGetIntegerv(GL_SCISSOR_BOX, box);
        r = Rect(box[0] / ratio, box[1] / ratio,
                 box[2] / ratio, box[3] / ratio);

        return scrTest ? true : false;
    }

    void VideoDriverGL::setRenderTarget(spNativeTexture rt)
    {
        m_rt = safeSpCast<NativeTextureGLES>(rt);
    }

    void VideoDriverGL::_begin(const Rect& viewport, const QColor* clearColor)
    {
        GameWindow* window = oxygine::GameWindow::getWindow();
        qreal ratio = window->devicePixelRatio();
        window->glViewport(viewport.getX() * ratio, viewport.getY() * ratio,
                           viewport.getWidth() * ratio, viewport.getHeight() * ratio);
        window->glDisable(GL_SCISSOR_TEST);
        if (clearColor)
        {
            Vector4 c = Vector4(clearColor->redF(), clearColor->greenF(), clearColor->blueF(), clearColor->alphaF());
            window->glClearColor(c.x, c.y, c.z, c.w);
            window->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
        else
        {
            window->glClear(GL_DEPTH_BUFFER_BIT);
        }
    }

    void VideoDriverGL::setBlendFunc(BLEND_TYPE src, BLEND_TYPE dest)
    {
        GameWindow* window = oxygine::GameWindow::getWindow();
        window->glBlendFunc(getBT(src), getBT(dest));
    }

    void VideoDriverGL::setState(STATE state, quint32 value)
    {
        GameWindow* window = oxygine::GameWindow::getWindow();
        switch (state)
        {
            case STATE_BLEND:
            {
                if (value)
                {
                    window->glEnable(GL_BLEND);
                }
                else
                {
                    window->glDisable(GL_BLEND);
                }
                break;
            }
            case STATE_CULL_FACE:
            {
                switch (value)
                {
                    case CULL_FACE_FRONT_AND_BACK:
                        window->glCullFace(GL_FRONT_AND_BACK);
                        break;
                    case CULL_FACE_FRONT:
                        window->glCullFace(GL_FRONT);
                        break;
                    case CULL_FACE_BACK:
                        window->glCullFace(GL_BACK);
                        break;
                    default:
                        break;
                }
                break;
            }
            default:
                oxygine::handleErrorPolicy(oxygine::ep_show_error, "VideoDriverGL::setState unknown state");
        }
    }
}
