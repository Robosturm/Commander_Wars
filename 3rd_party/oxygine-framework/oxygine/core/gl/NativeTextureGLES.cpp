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
        int format;
        int type;
        bool compressed;
    };

    glPixel SurfaceFormat2GL(ImageData::TextureFormat format)
    {
        glPixel pixel;
        pixel.compressed = false;
        //ADD_SF - dont remove this comment
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

        unsigned int f = GL_LINEAR;
        window->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, f);
        window->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, f);

        window->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        window->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        NativeTextureGLES::created++;

        return id;
    }

    NativeTextureGLES::NativeTextureGLES()
        : _id(0),
          _fbo(0),
          _format(ImageData::TF_UNDEFINED),
          _width(0),
          _height(0),
          _lockFlags(0)
    {

    }

    void NativeTextureGLES::init(int w, int h, ImageData::TextureFormat tf, bool rt)
    {
        release();

        GameWindow* window = oxygine::GameWindow::getWindow();

        GLuint id = createTexture();

        glPixel p = SurfaceFormat2GL(tf);
        window->glTexImage2D(GL_TEXTURE_2D, 0, p.format, w, h, 0, p.format, p.type, 0);

        if (rt)
        {
            int prevFBO = 0;
            window->glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prevFBO);
            window->glBindTexture(GL_TEXTURE_2D, 0);
            unsigned int fbo = 0;
            window->glGenFramebuffers(1, &fbo);
            window->glBindFramebuffer(GL_FRAMEBUFFER, fbo);
            window->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, (GLuint)id, 0);

            //printf("created fbo: %d\n", fbo);

            unsigned int status = window->glCheckFramebufferStatus(GL_FRAMEBUFFER);
            //qDebug("fbo status %d\n", status);
            //GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_OES
            if (status != GL_FRAMEBUFFER_COMPLETE)
            {
                qCritical("status != GL_FRAMEBUFFER_COMPLETE_OES");
            }

            window->glViewport(0, 0, w, h);
            window->glClearColor(0, 0, 0, 0);
            window->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            window->glBindFramebuffer(GL_FRAMEBUFFER, prevFBO);
            //qDebug("fbo bind\n");

            window->glBindTexture(GL_TEXTURE_2D, 0);

            _fbo = fbo;
        }

        _id = id;
        _width = w;
        _height = h;
        _format = tf;
    }

    void NativeTextureGLES::init(GLuint id, int w, int h, ImageData::TextureFormat tf)
    {
        release();
        _id = id;
        _width = w;
        _height = h;
        _format = tf;
    }

    void NativeTextureGLES::init(const ImageData& src, bool sysMemCopy)
    {
        GLuint id = createTexture();

        glPixel p = SurfaceFormat2GL(src.format);
        GameWindow* window = oxygine::GameWindow::getWindow();

        if (p.compressed)
        {
            window->glCompressedTexImage2D(GL_TEXTURE_2D, 0, p.format, src.w, src.h, 0, src.h * src.pitch, src.data);
        }
        else
        {
            window->glTexImage2D(GL_TEXTURE_2D, 0, p.format, src.w, src.h, 0, p.format, p.type, src.data);
        }

        Q_ASSERT(sysMemCopy == false);

        init(id, src.w, src.h, src.format);
    }

    void NativeTextureGLES::setLinearFilter(quint32 filter)
    {
        GameWindow* window = oxygine::GameWindow::getWindow();
        window->glActiveTexture(GL_TEXTURE7);
        window->glBindTexture(GL_TEXTURE_2D, (GLuint) _id);
        window->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        window->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
        window->glGenerateMipmap(GL_TEXTURE_2D);
    }

    void NativeTextureGLES::setClamp2Edge(bool clamp2edge)
    {
        GameWindow* window = oxygine::GameWindow::getWindow();
        window->glActiveTexture(GL_TEXTURE7);
        window->glBindTexture(GL_TEXTURE_2D, (GLuint) _id);

        unsigned int f = clamp2edge ? GL_CLAMP_TO_EDGE : GL_REPEAT;

        window->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, f);
        window->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, f);
    }

    void NativeTextureGLES::release()
    {
        GameWindow* window = oxygine::GameWindow::getWindow();
        if (_id)
        {
            NativeTextureGLES::created--;
            window->glDeleteTextures(1, (GLuint*)&_id);
            _id = 0;
        }


        if (_fbo)
        {
            window->glDeleteFramebuffers(1, (GLuint*)&_fbo);
            _fbo = 0;
        }

        unreg();
    }


    void NativeTextureGLES::swap(NativeTexture* t)
    {
        NativeTextureGLES* nt = safeCast<NativeTextureGLES*>(t);
        std::swap(_id, nt->_id);
        std::swap(_fbo, nt->_fbo);
        std::swap(_format, nt->_format);

        std::swap(_width, nt->_width);
        std::swap(_height, nt->_height);
        std::swap(_format, nt->_format);

    }

    NativeTextureGLES::~NativeTextureGLES()
    {
        release();
    }

    int NativeTextureGLES::getWidth() const
    {
        return _width;
    }

    int NativeTextureGLES::getHeight() const
    {
        return _height;
    }

    ImageData::TextureFormat NativeTextureGLES::getFormat() const
    {
        return _format;
    }

    ImageData NativeTextureGLES::lock(lock_flags flags, const Rect* src)
    {
        assert(_lockFlags == 0);


        _lockFlags = flags;
        Rect r(0, 0, _width, _height);

        if (src)
            r = *src;

        Q_ASSERT(r.getX() + r.getWidth() <= _width);
        Q_ASSERT(r.getY() + r.getHeight() <= _height);

        _lockRect = r;

        assert(_lockFlags != 0);

        if (_lockRect.isEmpty())
        {
            Q_ASSERT(!"_lockRect.IsEmpty()");
            return ImageData();
        }

        if (_data.empty())
        {
            //_data.resize(_width)
        }

        ImageData im =  ImageData(_width, _height, (int)(_data.size() / _height), _format, &_data.front());
        return im.getRect(_lockRect);
    }


    void NativeTextureGLES::unlock()
    {
        if (!_lockFlags)
            return;

        if (_lockFlags & lock_write)
        {
            GameWindow* window = oxygine::GameWindow::getWindow();
            window->glActiveTexture(GL_TEXTURE7);
            window->glBindTexture(GL_TEXTURE_2D, (GLuint) _id);
            window->glGetError();
            ImageData src = ImageData(_width, _height, (int)(_data.size() / _height), _format, &_data.front());
            ImageData locked = src.getRect(_lockRect);
            window->glGetError();
            Image mt;
            mt.init(_lockRect.getWidth(), _lockRect.getHeight(), _format);
            ImageData q = mt.lock();
            operations::copy(locked, q);
            mt.unlock();

            glPixel glp = SurfaceFormat2GL(_format);

            window->glTexSubImage2D(GL_TEXTURE_2D, 0,
                                    _lockRect.getX(), _lockRect.getY(), _lockRect.getWidth(), _lockRect.getHeight(),
                                    glp.format, glp.type, locked.data);
            window->glGetError();
            _lockFlags = 0;
        }
    }


    void NativeTextureGLES::updateRegion(int x, int y, const ImageData& data_)
    {
        if (data_.w == 0 || data_.h == 0)
            return;

        ImageData data = data_;
        assert(_width >= data.w - x);
        assert(_height >= data.h - y);
        GameWindow* window = oxygine::GameWindow::getWindow();
        window->glActiveTexture(GL_TEXTURE7);
        window->glBindTexture(GL_TEXTURE_2D, (GLuint) _id);

        glPixel glp = SurfaceFormat2GL(_format);
        //saveImage(data, "test1.png");


        Image mt;
        if (_format != data.format)
        {
            mt.init(data.w, data.h, _format);
            mt.fillZero();
            mt.updateRegion(0, 0, data);
            data = mt.lock();
        }

        window->glTexSubImage2D(GL_TEXTURE_2D, 0,
                                x, y, data.w, data.h,
                                glp.format, glp.type, data.data);
    }

    void NativeTextureGLES::apply(const Rect*)
    {

    }

    GLuint NativeTextureGLES::getHandle() const
    {
        return reinterpret_cast<GLuint>(_id);
    }

    unsigned int NativeTextureGLES::getFboID() const
    {
        return (unsigned int) _fbo;
    }
}
