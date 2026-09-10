#include "3rd_party/oxygine-framework/oxygine/core/opengl/texture.h"
#include "3rd_party/oxygine-framework/oxygine/core/gamewindow.h"

#include "3rd_party/oxygine-framework/oxygine/core/gamewindow.h"

#include "coreengine/interpreter.h"

namespace oxygine
{
GLuint Texture::m_highestTextureCount = 0;

Texture::Texture()
{
    Interpreter::setCppOwnerShip(this);
#ifdef GRAPHICSUPPORT
    setObjectName("Texture");
#endif
    Q_ASSERT(GameWindow::getWindow()->isRenderThread());
}

Texture::~Texture()
{
    Texture::release();
}

void Texture::setLinearFilter(quint32 filter)
{
    if (filter == 0)
    {
        filter = GL_NEAREST;
    }

    GameWindow* window = oxygine::GameWindow::getWindow();
    auto * gl = window->context()->extraFunctions();
    gl->glActiveTexture(GL_TEXTURE7);
    gl->glBindTexture(GL_TEXTURE_2D, (GLuint) m_id);
    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
    gl->glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::setClamp2Edge(bool clamp2edge)
{
    GameWindow* window = oxygine::GameWindow::getWindow();
    auto * gl = window->context()->extraFunctions();
    gl->glActiveTexture(GL_TEXTURE7);
    gl->glBindTexture(GL_TEXTURE_2D, (GLuint) m_id);

    GLint f = clamp2edge ? GL_CLAMP_TO_EDGE : GL_REPEAT;

    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, f);
    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, f);
}

GLuint Texture::createTexture()
{
    GameWindow* window = oxygine::GameWindow::getWindow();
    auto * gl = window->context()->extraFunctions();
    GLuint ids[1] = {0};
    gl->glGenTextures(1, ids);
    gl->glActiveTexture(GL_TEXTURE7);
    gl->glBindTexture(GL_TEXTURE_2D, ids[0]);

    GLint  f = GL_NEAREST;
    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, f);
    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, f);

    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    return ids[0];
}

quint32 Texture::getHighestTextureCount()
{
    return m_highestTextureCount;
}

void Texture::release()
{
    GameWindow* window = oxygine::GameWindow::getWindow();
    if (window != nullptr)
    {
        auto * gl = window->context()->extraFunctions();
        if (m_id > 0 && gl != nullptr)
        {
            GLuint ids[] = {m_id};
            gl->glDeleteTextures(1, ids);
            m_id = 0;
        }
    }
}

void Texture::init(const QImage & image)
{
    m_id = Texture::createTexture();
    if (m_id > m_highestTextureCount)
    {
        m_highestTextureCount = m_id;
    }
    m_image = image;
    GameWindow* window = oxygine::GameWindow::getWindow();
    auto * gl = window->context()->extraFunctions();
    gl->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_image.width(), m_image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_image.bits());
}
}
