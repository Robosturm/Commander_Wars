#include "3rd_party/oxygine-framework/oxygine/core/texture.h"
#include "3rd_party/oxygine-framework/oxygine/core/gamewindow.h"

namespace oxygine
{
    GLuint Texture::m_highestTextureCount = 0;
    Texture::~Texture()
    {
        Texture::release();
    }

    void Texture::setLinearFilter(quint32 filter)
    {
#ifdef GRAPHICSUPPORT
        GameWindow* window = oxygine::GameWindow::getWindow();
        window->glActiveTexture(GL_TEXTURE7);
        window->glBindTexture(GL_TEXTURE_2D, (GLuint) m_id);
        window->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
        window->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
        window->glGenerateMipmap(GL_TEXTURE_2D);
#endif
    }

    void Texture::setClamp2Edge(bool clamp2edge)
    {
#ifdef GRAPHICSUPPORT
        GameWindow* window = oxygine::GameWindow::getWindow();
        window->glActiveTexture(GL_TEXTURE7);
        window->glBindTexture(GL_TEXTURE_2D, (GLuint) m_id);

        GLint f = clamp2edge ? GL_CLAMP_TO_EDGE : GL_REPEAT;

        window->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, f);
        window->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, f);
#endif
    }

    GLuint Texture::createTexture()
    {
#ifdef GRAPHICSUPPORT
        GameWindow* window = oxygine::GameWindow::getWindow();
        GLuint ids[1] = {0};
        window->glGenTextures(1, ids);
        window->glActiveTexture(GL_TEXTURE7);
        window->glBindTexture(GL_TEXTURE_2D, ids[0]);

        GLint  f = GL_NEAREST;
        window->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, f);
        window->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, f);

        window->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        window->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        return ids[0];
#else
        return 0;
#endif
    }

    GLuint Texture::getHighestTextureCount()
    {
        return m_highestTextureCount;
    }

    void Texture::release()
    {
#ifdef GRAPHICSUPPORT
        GameWindow* window = oxygine::GameWindow::getWindow();
        if (m_id > 0)
        {
            GLuint ids[] = {m_id};
            window->glDeleteTextures(1, ids);
            m_id = 0;
        }
#endif
    }

    void Texture::init(const QImage & image)
    {
#ifdef GRAPHICSUPPORT
        m_id = Texture::createTexture();
        if (m_id > m_highestTextureCount)
        {
            m_highestTextureCount = m_id;
        }
        m_image = image;
        GameWindow* window = oxygine::GameWindow::getWindow();
        window->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_image.width(), m_image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_image.bits());
#endif
    }
}
