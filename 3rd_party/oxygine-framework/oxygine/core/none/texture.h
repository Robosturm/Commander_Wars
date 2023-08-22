#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"

#include <QImage>

namespace oxygine
{
class Texture;
using spTexture = std::shared_ptr<Texture>;
class Texture final
{
public:
    ~Texture();
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
    friend class std::shared_ptr<Texture>;
    explicit Texture() = default;
};
}
