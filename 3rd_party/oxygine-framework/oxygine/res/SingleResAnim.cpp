#include "3rd_party/oxygine-framework/oxygine/res/SingleResAnim.h"
#include "3rd_party/oxygine-framework/oxygine/Image.h"
#include "3rd_party/oxygine-framework/oxygine/core/NativeTexture.h"
#include "3rd_party/oxygine-framework/oxygine/core/VideoDriver.h"

#include "qthread.h"
#include "qapplication.h"

namespace oxygine
{

    void makeAlpha(const ImageData& srcImage, Rect& bounds, QVector<unsigned char>& alpha, HitTestData& adata, bool hittest);

    SingleResAnim::SingleResAnim()
    {
        moveToThread(QApplication::instance()->thread());
        setObjectName("SingleResAnim");
    }

    void SingleResAnim::init(QString file, qint32 columns /* = 1 */, qint32 rows /* = 1 */, float scaleFactor /* = 1.0f */)
    {
        ResAnim::init(file, columns, rows, scaleFactor);
    }

    void SingleResAnim::init(Image* original, qint32 columns, qint32 rows, float scaleFactor)
    {

        m_scaleFactor = scaleFactor;
        if (!original)
        {
            return;
        }
        spNativeTexture texture = IVideoDriver::instance->createTexture();
        m_texture = texture;

        texture->init(original->lock(), false);
        texture->apply();
        ResAnim::init(texture, original->getSize(), columns, rows, scaleFactor);



        Point originalSize = original->getSize();

        qint32 frame_width = originalSize.x / columns;
        qint32 frame_height = originalSize.y / rows;

        Vector2 frameSize((float)frame_width, (float)frame_height);
        qint32 i = 0;
        for (qint32 y = 0; y < rows; ++y)
        {
            for (qint32 x = 0; x < columns; ++x)
            {
                Rect src;
                src.pos = Point(x * frame_width, y * frame_height);
                src.size = Point(frame_width, frame_height);


                Rect bounds;
                HitTestData ht;
                const ImageData& im = original->lock(src);
                makeAlpha(im, bounds, m_data, ht, true);

                m_frames[i].setHitTestData(ht);
                ++i;
            }
        }

        i = 0;
        for (qint32 y = 0; y < rows; ++y)
        {
            for (qint32 x = 0; x < columns; ++x)
            {
                HitTestData ht = m_frames[i].getHitTestData();
                if (m_data.length() > 0)
                {
                    ht.data = &m_data[reinterpret_cast<size_t>(ht.data)];
                }
                m_frames[i].setHitTestData(ht);
                ++i;
            }
        }

    }

    spNativeTexture SingleResAnim::getTexture() const
    {
        return m_texture;
    }

}
