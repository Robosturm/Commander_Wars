#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-include.h"
#include "3rd_party/oxygine-framework/oxygine/core/NativeTexture.h"
#include <QOpenGLShader>

namespace oxygine
{
    DECLARE_SMART(NativeTextureGLES, spNativeTextureGLES);
    class NativeTextureGLES : public NativeTexture
    {
    public:
        ~NativeTextureGLES();

        virtual void init(GLuint id, int w, int h, ImageData::TextureFormat tf) override;
        virtual void init(int w, int h, ImageData::TextureFormat tf, bool renderTarget) override;
        virtual void init(const ImageData& src, bool sysMemCopy) override;
        virtual void release() override;
        virtual void swap(NativeTexture*)  override;

        virtual GLuint getHandle() const override;
        virtual int    getWidth() const override;
        virtual int    getHeight() const override;
        virtual ImageData::TextureFormat getFormat() const override;
        unsigned int        getFboID() const;

        virtual ImageData lock(lock_flags, const Rect* src) override;
        virtual void unlock() override;

        void setLinearFilter(quint32 filter) override;
        virtual void setClamp2Edge(bool clamp2edge) override;

        virtual void updateRegion(int x, int y, const ImageData& data) override;
        virtual void apply(const Rect* rect = 0) override;

        GLuint getId() override
        {
            return _id;
        }
    protected:
        virtual void* _getRestorableObject() override {return this;}
        friend class VideoDriverGL;
        friend class VideoDriverGLES20;
        NativeTextureGLES();

        GLuint _id;
        size_t _fbo;

        ImageData::TextureFormat _format;
        int _width;
        int _height;


        QVector<unsigned char> _data;
        int _lockFlags;
        Rect _lockRect;
    };
}
