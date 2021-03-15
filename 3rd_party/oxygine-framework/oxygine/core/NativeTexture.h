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
        virtual nativeTextureHandle getHandle() const = 0;

        static QVector<spNativeTexture> getCreatedTextures();
        /**debug counter of created textures*/
        static volatile int created;
    };

    class NativeTextureNull: public NativeTexture
    {
    public:
        void init(nativeTextureHandle, int w, int h, ImageData::TextureFormat tf);
        void init(int w, int h, ImageData::TextureFormat tf, bool renderTarget = false);
        void init(const ImageData& src, bool sysMemCopy);
        void release();

        ImageData lock(lock_flags, const Rect* src);
        void unlock();
        void swap(NativeTexture*);

        void setClamp2Edge(bool clamp2edge);
        void setLinearFilter(quint32 filter) override;
        void updateRegion(int x, int y, const ImageData& data);
        void apply(const Rect* rect = 0);

        /**returns handle (ptr) to HW texture ID*/
        nativeTextureHandle getHandle() const;
        int getWidth() const;
        int getHeight() const;
        ImageData::TextureFormat getFormat() const;

        void* _getRestorableObject() { return 0; }
    };
}
