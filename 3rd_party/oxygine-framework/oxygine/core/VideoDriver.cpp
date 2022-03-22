#include "3rd_party/oxygine-framework/oxygine/core/VideoDriver.h"
#include "3rd_party/oxygine-framework/oxygine/core/ShaderProgram.h"
#include "3rd_party/oxygine-framework/oxygine/core/gamewindow.h"

namespace oxygine
{
    spVideoDriver VideoDriver::instance;
    VideoDriver::Stats VideoDriver::m_stats;
    VertexDeclaration VideoDriver::m_VertexDeclaration;

    void VideoDriver::setUniform(const char* id, const QMatrix4x4& mat)
    {
        GameWindow* window = oxygine::GameWindow::getWindow();
        GLint  p = window->glGetUniformLocation(m_programID, id);
        if (p == -1)
        {
            return;
        }
        window->glUniformMatrix4fv(p, 1, GL_FALSE, mat.constData());
    }

    void VideoDriver::setUniform(const char* id, const Vector2& v)
    {
        setUniform(id, &v, 1);
    }

    void VideoDriver::setUniform(const char* id, const Uniform3f& v)
    {
        setUniform(id, &v, 1);
    }

    void VideoDriver::setUniform(const char* id, const Uniform4f& v)
    {
        setUniform(id, &v, 1);
    }

    VideoDriver::VideoDriver()
    {
        m_rt = spTexture::create();
    }

    VideoDriver::~VideoDriver()
    {
        GameWindow* window = oxygine::GameWindow::getWindow();
        window->glUseProgram(0);
    }

    quint32 VideoDriver::getPT(VideoDriver::PRIMITIVE_TYPE pt)
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
        oxygine::handleErrorPolicy(oxygine::ep_show_error, "VideoDriver::getPT unknown primitive type");
        return PT_POINTS;
    }

    quint32 VideoDriver::getBT(VideoDriver::BLEND_TYPE pt)
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
        oxygine::handleErrorPolicy(oxygine::ep_show_error, "VideoDriver::getBT unknown blend");
        return GL_ONE;
    }

    spTexture VideoDriver::getRenderTarget() const
    {
        return m_rt;
    }

    const VertexDeclaration* VideoDriver::getVertexDeclaration() const
    {
        return &m_VertexDeclaration;
    }

    void VideoDriver::getViewport(Rect& r) const
    {
        GLint vp[4];
        GameWindow* window = oxygine::GameWindow::getWindow();
        window->glGetIntegerv(GL_VIEWPORT, vp);
        qreal ratio = window->devicePixelRatio();
        r = Rect(vp[0] / ratio, vp[1] / ratio,
                 vp[2] / ratio, vp[3] / ratio);
    }

    void VideoDriver::setScissorRect(const Rect* rect)
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

    bool VideoDriver::getScissorRect(Rect& r) const
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

    void VideoDriver::setRenderTarget(spTexture & rt)
    {
        m_rt = rt;
    }

    void VideoDriver::_begin(const Rect& viewport, const QColor* clearColor)
    {
        GameWindow* window = oxygine::GameWindow::getWindow();
        qreal ratio = window->devicePixelRatio();
        window->glViewport(viewport.getX() * ratio, viewport.getY() * ratio,
                           viewport.getWidth() * ratio, viewport.getHeight() * ratio);
        window->glDisable(GL_SCISSOR_TEST);
        if (clearColor)
        {
            window->glClearColor(clearColor->redF(), clearColor->greenF(), clearColor->blueF(), clearColor->alphaF());
            window->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
        else
        {
            window->glClear(GL_DEPTH_BUFFER_BIT);
        }
    }

    void VideoDriver::setBlendFunc(BLEND_TYPE src, BLEND_TYPE dest)
    {
        GameWindow* window = oxygine::GameWindow::getWindow();
        window->glBlendFunc(getBT(src), getBT(dest));
    }

    void VideoDriver::setState(STATE state, quint32 value)
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
            default:
                oxygine::handleErrorPolicy(oxygine::ep_show_error, "VideoDriver::setState unknown state");
        }
    }

    void VideoDriver::restore()
    {
        setDefaultSettings();
    }

    bool VideoDriver::isReady() const
    {
        return true;
    }

    spTexture VideoDriver::createTexture()
    {
        return spTexture::create();
    }

    void VideoDriver::reset()
    {
    }

    void VideoDriver::begin(const Rect& viewport, const QColor* clearColor)
    {


        _begin(viewport, clearColor);
    }

    void VideoDriver::clear(const QColor& color)
    {
        GameWindow* window = oxygine::GameWindow::getWindow();
        window->glClearColor(color.redF(), color.greenF(), color.blueF(), color.alphaF());
        window->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void VideoDriver::setViewport(const Rect& viewport)
    {
        GameWindow* window = oxygine::GameWindow::getWindow();
        qreal ratio = window->devicePixelRatio();
        window->glViewport(viewport.getX() * ratio, viewport.getY() * ratio,
                           viewport.getWidth() * ratio, viewport.getHeight() * ratio);
    }

    void VideoDriver::setShaderProgram(ShaderProgram* prog_)
    {
        prog_->bind();
        m_programID = prog_->getID();
        m_pShaderProgram = prog_;
    }

    void VideoDriver::setTexture(qint32 sampler, spTexture & t)
    {
        GameWindow* window = oxygine::GameWindow::getWindow();
        window->glActiveTexture(GL_TEXTURE0 + sampler);
        if (t)
        {
            window->glBindTexture(GL_TEXTURE_2D, t->getHandle());
        }
        else
        {
            window->glBindTexture(GL_TEXTURE_2D, 0);
        }
    }

    void VideoDriver::draw(PRIMITIVE_TYPE pt, const VertexDeclaration* decl, const VertexPCT2* verticesData, GLsizei primitives)
    {
        OXY_ASSERT(primitives > 0);
        GameWindow* window = oxygine::GameWindow::getWindow();
        const unsigned char* vData = reinterpret_cast<const unsigned char*>(verticesData);
        for (const auto & el : decl->m_elements)
        {
            window->glEnableVertexAttribArray(el.index);
            window->glVertexAttribPointer(el.index, el.size, el.elemType, el.normalized, decl->size, vData + el.offset);
        }
        window->glDrawArrays(getPT(pt), 0, primitives);
        for (const auto & el : decl->m_elements)
        {
            window->glDisableVertexAttribArray(el.index);
        }
    }

    void VideoDriver::draw(PRIMITIVE_TYPE pt, const VertexDeclaration* decl, const VertexPCT2* verticesData, const quint16* indicesData, quint32 numIndices)
    {
        GameWindow* window = oxygine::GameWindow::getWindow();
        const unsigned char* vData = reinterpret_cast<const unsigned char*>(verticesData);
        for (const auto & el : decl->m_elements)
        {
            window->glEnableVertexAttribArray(el.index);
            window->glVertexAttribPointer(el.index, el.size, el.elemType, el.normalized, decl->size, vData + el.offset);
        }

        window->glDrawElements(getPT(pt), numIndices, GL_UNSIGNED_SHORT, indicesData);

        for (const auto & el : decl->m_elements)
        {
            window->glDisableVertexAttribArray(el.index);
        }
    }

    void VideoDriver::setDefaultSettings()
    {
        GameWindow* window = oxygine::GameWindow::getWindow();
        window->glDisable(GL_SCISSOR_TEST);
        window->glDisable(GL_DEPTH_TEST);
        window->glDisable(GL_BLEND);
        window->glDepthMask(GL_FALSE);
        window->glCullFace(GL_FRONT_AND_BACK);
#ifdef GL_MULTISAMPLE
        window->glEnable(GL_MULTISAMPLE);
#endif
    }

    // GL_BLEND
    // texture?

    void VideoDriver::setUniformInt(const char* id, qint32 v)
    {
        GameWindow* window = oxygine::GameWindow::getWindow();
        GLint location = window->glGetUniformLocation(m_programID, id);
        if (location == -1)
        {
            return;
        }
        window->glUniform1i(location, v);
    }

    void VideoDriver::setUniform(const char* id, const Uniform4f* v, qint32 num)
    {
        GameWindow* window = oxygine::GameWindow::getWindow();
        GLint p = window->glGetUniformLocation(m_programID, id);
        if (p == -1)
        {
            return;
        }
        window->glUniform4fv(p, num, v->data);
    }

    void VideoDriver::setUniform(const char* id, const Vector2* v, qint32 num)
    {
        GameWindow* window = oxygine::GameWindow::getWindow();
        GLint  p = window->glGetUniformLocation(m_programID, id);
        if (p == -1)
        {
            return;
        }
        window->glUniform2fv(p, num, &v->x);
    }

    void VideoDriver::setUniform(const char* id, const Uniform3f* v, qint32 num)
    {
        GameWindow* window = oxygine::GameWindow::getWindow();
        GLint  p = window->glGetUniformLocation(m_programID, id);
        if (p == -1)
        {
            return;
        }
        window->glUniform3fv(p, num, v->data);
    }

    void VideoDriver::setUniform(const char* id, float val)
    {
        GameWindow* window = oxygine::GameWindow::getWindow();
        GLint  p = window->glGetUniformLocation(m_programID, id);
        if (p == -1)
        {
            return;
        }
        window->glUniform1f(p, val);
    }
}
