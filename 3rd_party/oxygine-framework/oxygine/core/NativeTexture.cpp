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


    void NativeTextureNull::init(nativeTextureHandle, int, int, ImageData::TextureFormat)
    {

    }

    void NativeTextureNull::init(int, int, ImageData::TextureFormat, bool)
    {

    }

    void NativeTextureNull::init(const ImageData&, bool)
    {

    }

    void NativeTextureNull::release()
    {

    }

    ImageData NativeTextureNull::lock(lock_flags, const Rect*)
    {
        return ImageData();
    }

    void NativeTextureNull::unlock()
    {

    }

    void NativeTextureNull::swap(NativeTexture*)
    {

    }

    void NativeTextureNull::setClamp2Edge(bool)
    {

    }

    void NativeTextureNull::setLinearFilter(quint32 filter)
    {

    }

    void NativeTextureNull::updateRegion(int, int, const ImageData&)
    {

    }

    void NativeTextureNull::apply(const Rect*)
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
