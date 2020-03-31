#include "NativeTexture.h"
#include "ImageDataOperations.h"
#include "../Image.h"
#include <stdio.h>

namespace oxygine
{
    volatile int NativeTexture::created = 0;

    QVector<spNativeTexture> NativeTexture::getCreatedTextures()
    {

        QVector<spNativeTexture> t;

        const ObjectBase::__createdObjects& obj = ObjectBase::__getCreatedObjects();
        for (ObjectBase::__createdObjects::const_iterator i = obj.begin(); i != obj.end(); ++i)
        {
            ObjectBase* ob = *i;
            NativeTexture* nt = dynamic_cast<NativeTexture*>(ob);
            if (!nt)
                continue;
            if (!nt->getHandle())
                continue;

            t.push_back(nt);
        }
        return t;
    }


    void NativeTextureNull::init(nativeTextureHandle, int w, int h, ImageData::TextureFormat tf)
    {

    }

    void NativeTextureNull::init(int w, int h, ImageData::TextureFormat tf, bool renderTarget)
    {

    }

    void NativeTextureNull::init(const ImageData& src, bool sysMemCopy)
    {

    }

    void NativeTextureNull::release()
    {

    }

    ImageData NativeTextureNull::lock(lock_flags, const Rect* src)
    {
        return ImageData();
    }

    void NativeTextureNull::unlock()
    {

    }

    void NativeTextureNull::swap(NativeTexture*)
    {

    }

    void NativeTextureNull::setClamp2Edge(bool clamp2edge)
    {

    }

    void NativeTextureNull::setLinearFilter(bool enable)
    {

    }

    void NativeTextureNull::updateRegion(int x, int y, const ImageData& data)
    {

    }

    void NativeTextureNull::apply(const Rect* rect)
    {

    }

    /**returns handle (ptr) to HW texture ID*/
    nativeTextureHandle NativeTextureNull::getHandle() const
    {
        return nullptr;
    }

    int NativeTextureNull::getWidth() const
    {
        return 0;
    }

    int NativeTextureNull::getHeight() const
    {
        return 0;
    }
    ImageData::TextureFormat NativeTextureNull::getFormat() const
    {
        return ImageData::TF_UNDEFINED;
    }
}
