#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-include.h"
#include "3rd_party/oxygine-framework/oxygine/core/NativeTexture.h"
#include <QOpenGLShader>

namespace oxygine
{
    class NativeTextureGLES;
    using spNativeTextureGLES = intrusive_ptr<NativeTextureGLES>;

    class NativeTextureGLES : public NativeTexture
    {
    public:
        ~NativeTextureGLES();

        virtual void init(GLuint id, qint32 w, qint32 h, ImageData::TextureFormat tf) override;
        virtual void init(qint32 w, qint32 h, ImageData::TextureFormat tf, bool renderTarget) override;
        virtual void init(const ImageData& src, bool sysMemCopy) override;
        virtual void release() override;
        virtual void swap(NativeTexture*)  override;

        virtual GLuint getHandle() const override;
        virtual qint32    getWidth() const override;
        virtual qint32    getHeight() const override;
        virtual ImageData::TextureFormat getFormat() const override;
        quint32        getFboID() const;

        virtual ImageData lock(lock_flags, const Rect* src) override;
        virtual void unlock() override;

        void setLinearFilter(quint32 filter) override;
        virtual void setClamp2Edge(bool clamp2edge) override;

        virtual void updateRegion(qint32 x, qint32 y, const ImageData& data) override;
        virtual void apply(const Rect* rect = 0) override;

        GLuint getId() override
        {
            return m_id;
        }
    protected:
        virtual Restorable* _getRestorableObject() override {return this;}
        friend class VideoDriverGL;
        friend class VideoDriverGLES20;
        friend class intrusive_ptr<NativeTextureGLES>;
        NativeTextureGLES();

    protected:
        GLuint m_id;
        size_t m_fbo;
        ImageData::TextureFormat m_format;
        qint32 m_width;
        qint32 m_height;
        QVector<unsigned char> m_data;
        qint32 m_lockFlags;
        Rect m_lockRect;
    };
}
