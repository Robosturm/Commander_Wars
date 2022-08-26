#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/core/Object.h"
#include "3rd_party/oxygine-framework/oxygine/core/ref_counter.h"
#include "3rd_party/oxygine-framework/oxygine/math/Rect.h"

#include <QObject>
#include <QImage>

namespace oxygine
{
    class Texture;
    using spTexture = intrusive_ptr<Texture>;
    class Texture : public QObject, public Object
    {
        Q_OBJECT
    public:
        virtual ~Texture();
        void init(const QImage & image);

        qint32 getWidth() const
        {
            return 0;
        }
        qint32 getHeight() const
        {
            return 0;
        }
        quint32 getHandle() const
        {
            return 0;
        }
        /**Disabled/Enables bilinear texture filter*/
        void setLinearFilter(quint32 filter);
        /**set wrap mode as clamp2edge or repeat (tiling)*/
        void setClamp2Edge(bool clamp2edge);

        timeMS getCreationTime()
        {
            return timeMS();
        }
        void setCreationTime(timeMS time)
        {
        }
        void release();
        static quint32 getHighestTextureCount();
    protected:
        friend class VideoDriver;
        friend class intrusive_ptr<Texture>;
        explicit Texture() = default;
    private:
    };
}
