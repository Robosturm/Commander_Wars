#include "3rd_party/oxygine-framework/oxygine/core/gl/VideoDriverGLES20.h"
#include "3rd_party/oxygine-framework/oxygine/core/gl/NativeTextureGLES.h"
#include "3rd_party/oxygine-framework/oxygine/core/gl/ShaderProgramGL.h"

#include "3rd_party/oxygine-framework/oxygine/core/vertex.h"
#include "3rd_party/oxygine-framework/oxygine/core/gamewindow.h"

namespace oxygine
{

    VideoDriverGLES20::VideoDriverGLES20(): _programID(0), _p(0)
    {
    }

    VideoDriverGLES20::~VideoDriverGLES20()
    {
        GameWindow* window = oxygine::GameWindow::getWindow();
        window->glUseProgram(0);
    }

    void VideoDriverGLES20::restore()
    {
        //_us.restore();
        setDefaultSettings();
    }

    bool VideoDriverGLES20::isReady() const
    {
        return true;
    }

    spNativeTexture VideoDriverGLES20::createTexture()
    {
        return new NativeTextureGLES;
    }

    void VideoDriverGLES20::reset()
    {
        //_currentProgram = 0;
    }


    void VideoDriverGLES20::begin(const Rect& viewport, const QColor* clearColor)
    {
        _begin(viewport, clearColor);
    }

    void VideoDriverGLES20::clear(const QColor& color)
    {
        GameWindow* window = oxygine::GameWindow::getWindow();
        Vector4 c = Vector4(color.redF(), color.greenF(), color.blueF(), color.alphaF());
        window->glClearColor(c.x, c.y, c.z, c.w);
        window->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void VideoDriverGLES20::setViewport(const Rect& viewport)
    {
        GameWindow* window = oxygine::GameWindow::getWindow();
        window->glViewport(viewport.getX(), viewport.getY(), viewport.getWidth(), viewport.getHeight());
    }

    void VideoDriverGLES20::setShaderProgram(ShaderProgram* prog_)
    {
        prog_->bind();
        _programID = prog_->getID();
        _p = prog_;
    }

    void VideoDriverGLES20::setTexture(int sampler, spNativeTexture t)
    {
        GameWindow* window = oxygine::GameWindow::getWindow();
        window->glActiveTexture(GL_TEXTURE0 + sampler);
        if (t)
        {
            window->glBindTexture(GL_TEXTURE_2D, (GLuint)((size_t)t->getHandle()));
        }
        else
        {
            window->glBindTexture(GL_TEXTURE_2D, 0);
        }
    }



    void VideoDriverGLES20::draw(PRIMITIVE_TYPE pt, const VertexDeclaration* decl_, const void* vdata, unsigned int verticesDataSize)
    {
        Q_ASSERT(verticesDataSize > 0);
        Q_ASSERT((verticesDataSize % decl_->size) == 0);
        GameWindow* window = oxygine::GameWindow::getWindow();

        const VertexDeclarationGL* decl = static_cast<const VertexDeclarationGL*>(decl_);

        const unsigned char* verticesData = (const unsigned char*)vdata;

        const VertexDeclarationGL::Element* el = decl->elements;
        for (int i = 0; i < decl->numElements; ++i)
        {
            window->glEnableVertexAttribArray(el->index);
            window->glVertexAttribPointer(el->index, el->size, el->elemType, el->normalized, decl->size, verticesData + el->offset);
            el++;
        }

        size_t primitives = verticesDataSize / decl->size;
        window->glDrawArrays(getPT(pt), 0, (GLsizei)primitives);

        el = decl->elements;
        for (int i = 0; i < decl->numElements; ++i)
        {
            window->glDisableVertexAttribArray(el->index);
            el++;
        }
    }

    void VideoDriverGLES20::draw(PRIMITIVE_TYPE pt, const VertexDeclaration* decl_, const void* vdata, unsigned int verticesDataSize, const unsigned short* indicesData, unsigned int numIndices)
    {
        Q_ASSERT(verticesDataSize > 0);
        Q_ASSERT((verticesDataSize % decl_->size) == 0);
        GameWindow* window = oxygine::GameWindow::getWindow();

        const VertexDeclarationGL* decl = static_cast<const VertexDeclarationGL*>(decl_);

        const unsigned char* verticesData = (const unsigned char*)vdata;

        const VertexDeclarationGL::Element* el = decl->elements;
        for (int i = 0; i < decl->numElements; ++i)
        {
            window->glEnableVertexAttribArray(el->index);
            window->glVertexAttribPointer(el->index, el->size, el->elemType, el->normalized, decl->size, verticesData + el->offset);
            el++;
        }

        window->glDrawElements(getPT(pt), numIndices, GL_UNSIGNED_SHORT, indicesData);

        el = decl->elements;
        for (int i = 0; i < decl->numElements; ++i)
        {
            window->glDisableVertexAttribArray(el->index);
            el++;
        }
    }

    void VideoDriverGLES20::setDefaultSettings()
    {
        GameWindow* window = oxygine::GameWindow::getWindow();
        window->glDisable(GL_SCISSOR_TEST);
        window->glDisable(GL_DEPTH_TEST);
        window->glDisable(GL_BLEND);
        window->glDepthMask(GL_FALSE);
        window->glCullFace(GL_FRONT_AND_BACK);
        window->glEnable(GL_MULTISAMPLE);
    }

    void VideoDriverGLES20::setUniformInt(const char* id, int v)
    {
        GameWindow* window = oxygine::GameWindow::getWindow();
        int location = window->glGetUniformLocation(_programID, id);
        if (location == -1)
        {
            return;
        }
        window->glUniform1i(location, v);
    }

    void VideoDriverGLES20::setUniform(const char* id, const Vector4* v, int num)
    {
        GameWindow* window = oxygine::GameWindow::getWindow();
        int p = window->glGetUniformLocation(_programID, id);
        if (p == -1)
        {
            return;
        }
        window->glUniform4fv(p, num, v->m);
    }

    void VideoDriverGLES20::setUniform(const char* id, const Vector2* v, int num)
    {
        GameWindow* window = oxygine::GameWindow::getWindow();
        int p = window->glGetUniformLocation(_programID, id);
        if (p == -1)
        {
            return;
        }
        window->glUniform2fv(p, num, &v->x);
    }

    void VideoDriverGLES20::setUniform(const char* id, const Vector3* v, int num)
    {
        GameWindow* window = oxygine::GameWindow::getWindow();
        int p = window->glGetUniformLocation(_programID, id);
        if (p == -1)
        {
            return;
        }
        window->glUniform3fv(p, num, &v->x);
    }

    void VideoDriverGLES20::setUniform(const char* id, const Matrix* mat, int num)
    {
        GameWindow* window = oxygine::GameWindow::getWindow();
        int p = window->glGetUniformLocation(_programID, id);
        if (p == -1)
        {
            return;
        }
        window->glUniformMatrix4fv(p, num, GL_FALSE, mat->ml);
    }

    void VideoDriverGLES20::setUniform(const char* id, float val)
    {
        GameWindow* window = oxygine::GameWindow::getWindow();
        int p = window->glGetUniformLocation(_programID, id);
        if (p == -1)
        {
            return;
        }
        window->glUniform1f(p, val);
    }
}
