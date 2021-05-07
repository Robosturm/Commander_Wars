#include "3rd_party/oxygine-framework/oxygine/res/ResAnim.h"
#include "3rd_party/oxygine-framework/oxygine/res/Resources.h"
#include "3rd_party/oxygine-framework/oxygine/Image.h"
#include "3rd_party/oxygine-framework/oxygine/core/NativeTexture.h"
#include "3rd_party/oxygine-framework/oxygine/core/VideoDriver.h"
#include "spritingsupport/spritecreator.h"

namespace oxygine
{
    AnimationFrame ResAnim::m_emptyFrame;
    ResAnim::ResAnim(Resource* atlas)
        : m_columns(1),
          m_atlas(atlas),
          m_scaleFactor(1.0f),
          m_appliedScale(1.0f),
          m_framerate(30)
    {
    }

    ResAnim::~ResAnim()
    {
    }

    void ResAnim::init(spNativeTexture texture, const Point& originalSize, qint32 columns, qint32 rows, float scaleFactor)
    {
        m_scaleFactor = scaleFactor;
        if (!texture)
        {
            return;
        }
        qint32 frame_width = originalSize.x / columns;
        qint32 frame_height = originalSize.y / rows;
        if (rows > 1 || columns > 1)
        {
            frame_height -= 1;
            frame_width -= 1;
        }
        float iw = 1.0f / static_cast<float>(columns);
        float ih = 1.0f / static_cast<float>(rows);
        float width = static_cast<float>(frame_width) / static_cast<float>(originalSize.x);
        float height = static_cast<float>(frame_height) / static_cast<float>(originalSize.y);

        animationFrames frames;
        qint32 frames_count = rows * columns;
        frames.reserve(frames_count);

        Vector2 frameSize(static_cast<float>(frame_width), static_cast<float>(frame_height));
        for (qint32 y = 0; y < rows; ++y)
        {
            for (qint32 x = 0; x < columns; ++x)
            {
                RectF srcRect(x * iw, y * ih, width, height);
                RectF destRect(Vector2(0, 0), frameSize * scaleFactor);
                AnimationFrame frame;
                Diffuse df;
                df.base = texture;
                frame.init(this, df, srcRect, destRect, destRect.size);
                frames.push_back(frame);
            }
        }

        init(frames, columns, scaleFactor);
    }

    void ResAnim::init(QString file, qint32 columns, qint32 rows, float scaleFactor)
    {
        QImage img(file);
        SpriteCreator::addTransparentBorder(img, columns, rows);
        Image mt;
        mt.init(img, true);
        init(&mt, columns, rows, scaleFactor);
    }

    void ResAnim::init(QImage & img, qint32 columns, qint32 rows, float scaleFactor)
    {
        Image mt;
        SpriteCreator::addTransparentBorder(img, columns, rows);
        mt.init(img, true);
        init(&mt, columns, rows, scaleFactor);
    }

    void ResAnim::init(Image* original, qint32 columns, qint32 rows, float scaleFactor)
    {
        m_scaleFactor = scaleFactor;
        if (!original)
        {
            return;
        }

        spNativeTexture texture = IVideoDriver::instance->createTexture();
        texture->init(original->lock(), false);
        texture->apply();
        init(texture, original->getSize(), columns, rows, scaleFactor);
    }

    void ResAnim::init(animationFrames& frames, qint32 columns, float scaleFactor, float appliedScale)
    {
        m_columns = columns;
        m_frames.swap(frames);
        for (qint32 i = 0; i < m_frames.size(); ++i)
        {
            m_frames[i].setResAnim(this);
        }
        m_scaleFactor = scaleFactor;
        m_appliedScale = appliedScale;
    }

    void ResAnim::_load(LoadResourcesContext* c)
    {
        if (!m_atlas)
        {
            return;
        }
        m_atlas->load(c);
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
        return m_emptyFrame;
    }

    void ResAnim::setFrame(qint32 col, qint32 row, const AnimationFrame& frame)
    {
        qint32 i = row * m_columns + col;
        if (i < (int)m_frames.size())
        {
            m_frames[i] = frame;
        }
    }

    const Vector2&  ResAnim::getSize() const
    {
        Q_ASSERT(!m_frames.empty());
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
