#include "3rd_party/oxygine-framework/oxygine/core/gl/NativeTextureGLES.h"
#include "3rd_party/oxygine-framework/oxygine/core/gamewindow.h"
#include "3rd_party/oxygine-framework/oxygine/core/ImageDataOperations.h"
#include "3rd_party/oxygine-framework/oxygine/core/NativeTexture.h"
#include "3rd_party/oxygine-framework/oxygine/Image.h"

#ifndef GL_COMPRESSED_RGBA8_ETC2_EAC
#   define GL_COMPRESSED_RGBA8_ETC2_EAC      0x9278
#endif

namespace oxygine
{
    struct glPixel
    {
        qint32 format;
        qint32 type;
        bool compressed;
    };

    glPixel SurfaceFormat2GL(ImageData::TextureFormat format)
    {
        glPixel pixel;
        pixel.compressed = false;
        switch (format)
        {
            case ImageData::TF_R8G8B8A8:
                pixel.format = GL_RGBA;
                pixel.type = GL_UNSIGNED_BYTE;
                break;
            default:
                qCritical("unknown format: %d\n", format);
                Q_ASSERT(!"unknown format");
        }
        return pixel;
    }

    GLuint createTexture()
    {
        GameWindow* window = oxygine::GameWindow::getWindow();
        GLuint id = 0;
        window->glGenTextures(1, &id);
        window->glActiveTexture(GL_TEXTURE7);
        window->glBindTexture(GL_TEXTURE_2D, id);

        quint32 f = GL_LINEAR;
        window->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, f);
        window->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, f);

        window->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        window->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        return id;
    }

    NativeTextureGLES::NativeTextureGLES()
        : m_id(0),
          m_fbo(0),
          m_format(ImageData::TF_UNDEFINED),
          m_width(0),
          m_height(0),
          m_lockFlags(0)
    {

    }

    void NativeTextureGLES::init(qint32 w, qint32 h, ImageData::TextureFormat tf, bool rt)
    {
        release();

        GameWindow* window = oxygine::GameWindow::getWindow();

        GLuint id = createTexture();

        glPixel p = SurfaceFormat2GL(tf);
        window->glTexImage2D(GL_TEXTURE_2D, 0, p.format, w, h, 0, p.format, p.type, 0);

        if (rt)
        {
            qint32 prevFBO = 0;
            window->glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prevFBO);
            window->glBindTexture(GL_TEXTURE_2D, 0);
            quint32 fbo = 0;
            window->glGenFramebuffers(1, &fbo);
            window->glBindFramebuffer(GL_FRAMEBUFFER, fbo);
            window->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, (GLuint)id, 0);
            quint32 status = window->glCheckFramebufferStatus(GL_FRAMEBUFFER);
            if (status != GL_FRAMEBUFFER_COMPLETE)
            {
                qCritical("status != GL_FRAMEBUFFER_COMPLETE_OES");
            }

            window->glViewport(0, 0, w, h);
            window->glClearColor(0, 0, 0, 0);
            window->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            window->glBindFramebuffer(GL_FRAMEBUFFER, prevFBO);
            window->glBindTexture(GL_TEXTURE_2D, 0);

            m_fbo = fbo;
        }

        m_id = id;
        m_width = w;
        m_height = h;
        m_format = tf;
    }

    void NativeTextureGLES::init(GLuint id, qint32 w, qint32 h, ImageData::TextureFormat tf)
    {
        release();
        m_id = id;
        m_width = w;
        m_height = h;
        m_format = tf;
    }

    void NativeTextureGLES::init(const ImageData& src, bool sysMemCopy)
    {
        GLuint id = createTexture();

        glPixel p = SurfaceFormat2GL(src.m_format);
        GameWindow* window = oxygine::GameWindow::getWindow();

        if (p.compressed)
        {
            window->glCompressedTexImage2D(GL_TEXTURE_2D, 0, p.format, src.m_w, src.m_h, 0, src.m_h * src.m_pitch, src.m_data);
        }
        else
        {
            window->glTexImage2D(GL_TEXTURE_2D, 0, p.format, src.m_w, src.m_h, 0, p.format, p.type, src.m_data);
        }

        Q_ASSERT(sysMemCopy == false);

        init(id, src.m_w, src.m_h, src.m_format);
    }

    void NativeTextureGLES::setLinearFilter(quint32 filter)
    {
        GameWindow* window = oxygine::GameWindow::getWindow();
        window->glActiveTexture(GL_TEXTURE7);
        window->glBindTexture(GL_TEXTURE_2D, (GLuint) m_id);
        window->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        window->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
        window->glGenerateMipmap(GL_TEXTURE_2D);
    }

    void NativeTextureGLES::setClamp2Edge(bool clamp2edge)
    {
        GameWindow* window = oxygine::GameWindow::getWindow();
        window->glActiveTexture(GL_TEXTURE7);
        window->glBindTexture(GL_TEXTURE_2D, (GLuint) m_id);

        quint32 f = clamp2edge ? GL_CLAMP_TO_EDGE : GL_REPEAT;

        window->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, f);
        window->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, f);
    }

    void NativeTextureGLES::release()
    {
        GameWindow* window = oxygine::GameWindow::getWindow();
        if (m_id)
        {
            window->glDeleteTextures(1, (GLuint*)&m_id);
            m_id = 0;
        }
        if (m_fbo)
        {
            window->glDeleteFramebuffers(1, (GLuint*)&m_fbo);
            m_fbo = 0;
        }

        unreg();
    }


    void NativeTextureGLES::swap(NativeTexture* t)
    {
        NativeTextureGLES* nt = safeCast<NativeTextureGLES*>(t);
        std::swap(m_id, nt->m_id);
        std::swap(m_fbo, nt->m_fbo);
        std::swap(m_format, nt->m_format);

        std::swap(m_width, nt->m_width);
        std::swap(m_height, nt->m_height);
        std::swap(m_format, nt->m_format);
    }

    NativeTextureGLES::~NativeTextureGLES()
    {
        NativeTextureGLES::release();
    }

    qint32 NativeTextureGLES::getWidth() const
    {
        return m_width;
    }

    qint32 NativeTextureGLES::getHeight() const
    {
        return m_height;
    }

    ImageData::TextureFormat NativeTextureGLES::getFormat() const
    {
        return m_format;
    }

    ImageData NativeTextureGLES::lock(lock_flags flags, const Rect* src)
    {
        assert(m_lockFlags == 0);


        m_lockFlags = flags;
        Rect r(0, 0, m_width, m_height);

        if (src)
        {
            r = *src;
        }
        Q_ASSERT(r.getX() + r.getWidth() <= m_width);
        Q_ASSERT(r.getY() + r.getHeight() <= m_height);

        m_lockRect = r;

        assert(m_lockFlags != 0);

        if (m_lockRect.isEmpty())
        {
            Q_ASSERT(!"_lockRect.IsEmpty()");
            return ImageData();
        }

        ImageData im =  ImageData(m_width, m_height, (int)(m_data.size() / m_height), m_format, &m_data.front());
        return im.getRect(m_lockRect);
    }


    void NativeTextureGLES::unlock()
    {
        if (!m_lockFlags)
        {
            return;
        }
        if (m_lockFlags & lock_write)
        {
            GameWindow* window = oxygine::GameWindow::getWindow();
            window->glActiveTexture(GL_TEXTURE7);
            window->glBindTexture(GL_TEXTURE_2D, (GLuint) m_id);
            window->glGetError();
            ImageData src = ImageData(m_width, m_height, (int)(m_data.size() / m_height), m_format, &m_data.front());
            ImageData locked = src.getRect(m_lockRect);
            window->glGetError();
            Image mt;
            mt.init(m_lockRect.getWidth(), m_lockRect.getHeight(), m_format);
            ImageData q = mt.lock();
            operations::copy(locked, q);
            mt.unlock();

            glPixel glp = SurfaceFormat2GL(m_format);

            window->glTexSubImage2D(GL_TEXTURE_2D, 0,
                                    m_lockRect.getX(), m_lockRect.getY(), m_lockRect.getWidth(), m_lockRect.getHeight(),
                                    glp.format, glp.type, locked.m_data);
            window->glGetError();
            m_lockFlags = 0;
        }
    }


    void NativeTextureGLES::updateRegion(qint32 x, qint32 y, const ImageData& data_)
    {
        if (data_.m_w == 0 || data_.m_h == 0)
            return;

        ImageData data = data_;
        assert(m_width >= data.m_w - x);
        assert(m_height >= data.m_h - y);
        GameWindow* window = oxygine::GameWindow::getWindow();
        window->glActiveTexture(GL_TEXTURE7);
        window->glBindTexture(GL_TEXTURE_2D, (GLuint) m_id);

        glPixel glp = SurfaceFormat2GL(m_format);
        Image mt;
        if (m_format != data.m_format)
        {
            mt.init(data.m_w, data.m_h, m_format);
            mt.fillZero();
            mt.updateRegion(0, 0, data);
            data = mt.lock();
        }

        window->glTexSubImage2D(GL_TEXTURE_2D, 0,
                                x, y, data.m_w, data.m_h,
                                glp.format, glp.type, data.m_data);
    }

    void NativeTextureGLES::apply(const Rect*)
    {

    }

    GLuint NativeTextureGLES::getHandle() const
    {
        return reinterpret_cast<GLuint>(m_id);
    }

    quint32 NativeTextureGLES::getFboID() const
    {
        return (unsigned int) m_fbo;
    }
}
