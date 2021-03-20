#include "3rd_party/oxygine-framework/oxygine/core/NativeTexture.h"
#include "3rd_party/oxygine-framework/oxygine/core/ImageDataOperations.h"
#include "3rd_party/oxygine-framework/oxygine/Image.h"

namespace oxygine
{
    volatile int NativeTexture::created = 0;

    void NativeTextureNull::init(GLuint, int, int, ImageData::TextureFormat)
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

    void NativeTextureNull::setLinearFilter(quint32)
    {

    }

    void NativeTextureNull::updateRegion(int, int, const ImageData&)
    {

    }

    void NativeTextureNull::apply(const Rect*)
    {

    }

    /**returns handle (ptr) to HW texture ID*/
    GLuint NativeTextureNull::getHandle() const
    {
        return 0;
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
