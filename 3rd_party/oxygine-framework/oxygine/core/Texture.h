#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-include.h"
#include "3rd_party/oxygine-framework/oxygine/core/ImageData.h"
#include "3rd_party/oxygine-framework/oxygine/core/Object.h"
#include "3rd_party/oxygine-framework/oxygine/core/ref_counter.h"
#include "3rd_party/oxygine-framework/oxygine/math/Rect.h"

namespace oxygine
{
    DECLARE_SMART(Texture, spTexture);

    inline unsigned int nextPOT(unsigned int v)
    {
        v = v - 1;
        v = v | (v >> 1);
        v = v | (v >> 2);
        v = v | (v >> 4);
        v = v | (v >> 8);
        v = v | (v >> 16);
        return v + 1;
    }

    class Texture : public Object
    {
    public:
        enum
        {
            lock_read = 0x1,
            lock_write = 0x2,
            lock_write_on_apply = 0x4,
        };

        typedef int lock_flags;

        virtual int getWidth() const = 0;
        virtual int getHeight() const = 0;
        virtual ImageData::TextureFormat getFormat() const = 0;

        virtual ImageData lock(lock_flags, const Rect* src) = 0;
        virtual void unlock() = 0;
        virtual void updateRegion(int x, int y, const ImageData& data) = 0;
        virtual void apply(const Rect* rect = 0) = 0;

        timeMS getCreationTime()
        {
            return m_CreationTime;
        }

        void setCreationTime(timeMS time)
        {
            m_CreationTime = time;
        }
    private:
        timeMS m_CreationTime;
    };
}
