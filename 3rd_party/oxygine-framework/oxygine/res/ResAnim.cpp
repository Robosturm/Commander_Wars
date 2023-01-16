#include "3rd_party/oxygine-framework/oxygine/res/ResAnim.h"
#include "3rd_party/oxygine-framework/oxygine/res/Resources.h"
#include "3rd_party/oxygine-framework/oxygine/core/VideoDriver.h"
#include "3rd_party/oxygine-framework/oxygine/core/oxygine.h"

#include "spritingsupport/spritecreator.h"

namespace oxygine
{
    ResAnim::ResAnim(Resource* atlas)
        : m_columns(1),
          m_atlas(atlas),
          m_scaleFactor(1.0f),
          m_appliedScale(1.0f),
          m_framerate(30)
    {
    }

    void ResAnim::init(spTexture & texture, const QSize& originalSize, qint32 columns, qint32 rows, float scaleFactor, bool addTransparentBorder)
    {
        m_scaleFactor = scaleFactor;
        if (!texture)
        {
            return;
        }
        qint32 frame_width = originalSize.width() / columns;
        qint32 frame_height = originalSize.height() / rows;
        if (rows > 1 || columns > 1)
        {
            if (addTransparentBorder)
            {
                frame_height -= 1;
                frame_width -= 1;
            }
            else
            {
                frame_height -= 2;
                frame_width -= 2;
            }
        }
        float iw = 1.0f / static_cast<float>(columns);
        float ih = 1.0f / static_cast<float>(rows);
        float width = static_cast<float>(frame_width) / static_cast<float>(originalSize.width());
        float height = static_cast<float>(frame_height) / static_cast<float>(originalSize.height());

        m_frames.clear();
        qint32 frames_count = rows * columns;
        m_frames.resize(frames_count);

        Vector2 frameSize(static_cast<float>(frame_width), static_cast<float>(frame_height));
        for (qint32 y = 0; y < rows; ++y)
        {
            for (qint32 x = 0; x < columns; ++x)
            {
                RectF srcRect(x * iw, y * ih, width, height);
                RectF destRect(Vector2(0, 0), frameSize * scaleFactor);
                m_frames[x + y * columns].init(this, x, y, texture, srcRect, destRect, destRect.size.cast<Point>());
            }
        }
        init(columns, scaleFactor);
    }

    void ResAnim::init(const QString & file, qint32 columns, qint32 rows, float scaleFactor, bool addTransparentBorder)
    {
        QImage img(file);
        init(img, columns, rows, scaleFactor, addTransparentBorder);
    }

    void ResAnim::init(QImage & img,  qint32 columns, qint32 rows, float scaleFactor, bool addTransparentBorder,
                       bool clamp2Edge, quint32 linearFilter)
    {
        SpriteCreator::addFrameBorders(img, columns, rows, addTransparentBorder);
        SpriteCreator::convertToRgba(img);
        m_scaleFactor = scaleFactor;
        spTexture texture = VideoDriver::instance->createTexture();
        texture->init(img);
        texture->setClamp2Edge(clamp2Edge);
        texture->setLinearFilter(linearFilter);
        init(texture, img.size(), columns, rows, scaleFactor, addTransparentBorder);
    }

    void ResAnim::init(qint32 columns, float scaleFactor, float appliedScale)
    {
        m_columns = columns;
        for (auto & frame : m_frames)
        {
            frame.setResAnim(this);
        }
        m_scaleFactor = scaleFactor;
        m_appliedScale = appliedScale;
    }

    void ResAnim::_load()
    {
        if (!m_atlas)
        {
            return;
        }
        m_atlas->load();
    }

    void ResAnim::_unload()
    {
    }

    QString ResAnim::getResPath() const
    {
        return m_resPath;
    }

    void ResAnim::setResPath(const QString &value)
    {
        m_resPath = value;
    }

    void ResAnim::removeFrames()
    {
        m_frames.clear();
    }

    ResAnim::operator const AnimationFrame& ()
    {
        return getFrame(0, 0);
    }

    const Resources* ResAnim::getResources() const
    {
        const Resource* parent = getParent()->getParent();
        const Resources* p = safeCast<const Resources*>(parent);
        return p;
    }

    const AnimationFrame& ResAnim::getFrame(qint32 col, qint32 row) const
    {
        qint32 i = row * m_columns + col;
        return getFrame(i);
    }

    const AnimationFrame&   ResAnim::getFrame(qint32 index) const
    {
        if (index < (int)m_frames.size())
        {
            return m_frames[index];
        }
        static QScopedPointer<AnimationFrame> emptyFrame;
        if (emptyFrame.isNull())
        {
            emptyFrame.reset(new AnimationFrame());
        }
        return *emptyFrame.get();
    }

    void ResAnim::setFrame(qint32 col, qint32 row, const AnimationFrame& frame)
    {
        qint32 i = row * m_columns + col;
        if (i < (int)m_frames.size())
        {
            m_frames[i] = frame;
        }
    }

    const Point& ResAnim::getSize() const
    {
        if (m_frames.empty())
        {
            static Point dummy;
            return dummy;
        }
        return m_frames[0].getSize();
    }
    float   ResAnim::getWidth() const
    {
        return getSize().x;
    }

    float   ResAnim::getHeight() const
    {
        return getSize().y;
    }
}
