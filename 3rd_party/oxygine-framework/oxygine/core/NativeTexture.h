#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-include.h"
#include "3rd_party/oxygine-framework/oxygine/closure/closure.h"
#include "3rd_party/oxygine-framework/oxygine/core/Restorable.h"
#include "3rd_party/oxygine-framework/oxygine/core/Texture.h"
#include <QOpenGLShader>
#include <QtGlobal>

namespace oxygine
{
    class NativeTexture: public Texture, public Restorable
    {
    public:
        NativeTexture() {}
        virtual void init(GLuint, int w, int h, ImageData::TextureFormat tf) = 0;
        virtual void init(int w, int h, ImageData::TextureFormat tf, bool renderTarget = false) = 0;
        virtual void init(const ImageData& src, bool sysMemCopy = false) = 0;
        //virtual void release() = 0;
        virtual GLuint getId() = 0;

        virtual ImageData lock(lock_flags, const Rect* src) = 0;
        virtual void unlock() = 0;
        virtual void swap(NativeTexture*) = 0;

        /**Disabled/Enables bilinear texture filter*/
        virtual void setLinearFilter(quint32 filter) = 0;
        /**set wrap mode as clamp2edge or repeat (tiling)*/
        virtual void setClamp2Edge(bool clamp2edge) = 0;
        virtual void updateRegion(int x, int y, const ImageData& data) = 0;
        virtual void apply(const Rect* rect = 0) = 0;

        /**returns handle (ptr) to HW texture ID*/
        virtual GLuint getHandle() const = 0;

        static QVector<spNativeTexture> getCreatedTextures();
        /**debug counter of created textures*/
        static volatile int created;
    };

    class NativeTextureNull: public NativeTexture
    {
    public:
        virtual void init(GLuint, int w, int h, ImageData::TextureFormat tf) override;
        virtual void init(int w, int h, ImageData::TextureFormat tf, bool renderTarget = false) override;
        virtual void init(const ImageData& src, bool sysMemCopy) override;
        virtual void release() override;

        virtual ImageData lock(lock_flags, const Rect* src) override;
        virtual void unlock() override;
        virtual void swap(NativeTexture*) override;

        virtual void setClamp2Edge(bool clamp2edge) override;
        virtual void setLinearFilter(quint32 filter) override;
        virtual void updateRegion(int x, int y, const ImageData& data) override;
        virtual void apply(const Rect* rect = 0) override;

        /**returns handle (ptr) to HW texture ID*/
        virtual GLuint getHandle() const override;
        virtual int getWidth() const override;
        virtual int getHeight() const override;
        virtual ImageData::TextureFormat getFormat() const override;

        virtual void* _getRestorableObject() override { return 0; }
    };
}
