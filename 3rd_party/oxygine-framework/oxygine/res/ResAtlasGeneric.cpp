#include "3rd_party/oxygine-framework/oxygine/res/ResAtlasGeneric.h"
#include "3rd_party/oxygine-framework/oxygine/res/Resources.h"
#include "3rd_party/oxygine-framework/oxygine/Image.h"
#include "3rd_party/oxygine-framework/oxygine/core/ImageDataOperations.h"
#include "3rd_party/oxygine-framework/oxygine/core/VideoDriver.h"
#include "3rd_party/oxygine-framework/oxygine/utils/AtlasBuilder.h"

#include "spritingsupport/spritecreator.h"

#include <qvariant.h>
#include <qfile.h>

namespace oxygine
{

    constexpr qint32 defaultAtlasWidth = 2048;
    constexpr qint32 defaultAtlasHeight = 2048;

    struct atlas_data
    {
        spNativeTexture texture;
        Image mt;
        AtlasBuilder atlas;
    };

    qint32 roundUp(qint32 numToRound, qint32 multiple)
    {
        if (multiple == 0)
        {
            return numToRound;
        }
        qint32 remainder = numToRound % multiple;
        if (remainder == 0)
        {
            return numToRound;
        }
        return numToRound + multiple - remainder;
    }


    qint32 HIT_TEST_DOWNSCALE = 4;
    const qint32 ALIGN = sizeof(int32_t);
    const qint32 BITS = ALIGN * 8;

    void makeAlpha(const ImageData& srcImage, Rect& bounds, QVector<unsigned char>& alpha, HitTestData& adata, bool hittest)
    {
        qint32 w = srcImage.m_w;
        qint32 h = srcImage.m_h;

        size_t pos = alpha.size();
        adata.data = reinterpret_cast<unsigned char*>(pos);
        adata.w = roundUp(w, HIT_TEST_DOWNSCALE) / HIT_TEST_DOWNSCALE;
        adata.h = roundUp(h, HIT_TEST_DOWNSCALE) / HIT_TEST_DOWNSCALE;


        qint32 lineInts = roundUp(adata.w, BITS) / BITS;

        qint32 destPitch = lineInts * ALIGN;

        qint32 size = adata.h * destPitch;

        alpha.resize(pos + size + 10);


        const unsigned char* srcData = srcImage.m_data;
        qint32 srcStep = srcImage.m_bytespp;
        qint32 srcPitch = srcImage.m_pitch;

        unsigned char* destData = &alpha[pos];

        adata.pitch = destPitch;

        const unsigned char* srcRow = srcData;
        unsigned char* destRow = destData;


        qint32 minX = w;
        qint32 minY = h;
        qint32 maxX = 0;
        qint32 maxY = 0;

        bool hasAlpha = false;

        for (qint32 y = 0; y != h; y += 1)
        {
            const unsigned char* srcLine = srcRow;
            int32_t* destLine = reinterpret_cast<int32_t*>(destRow);

            bool lineWithAlpha = false;


            for (qint32 x = 0; x != w; x += 1)
            {
                PixelR8G8B8A8 pd;
                Pixel p;
                pd.getPixel(srcLine, p);
                if (p.a > 5)
                {
                    hasAlpha = true;

                    qint32 dx = x / HIT_TEST_DOWNSCALE;
                    qint32 n = dx / BITS;
                    qint32 b = dx % BITS;

                    destLine[n] |= 1 << b;

                    lineWithAlpha = true;
                    if (x > maxX)
                    {
                        maxX = x;
                    }
                    else if (x < minX)
                    {
                        minX = x;
                    }
                }
                srcLine += srcStep;
            }

            if (lineWithAlpha)
            {
                if (minY == h)
                    minY = y;
                maxY = y;
            }

            if (y % HIT_TEST_DOWNSCALE == HIT_TEST_DOWNSCALE - 1)
            {
                //reset line
                destRow += destPitch;
            }

            srcRow += srcPitch;
        }

        //if image is transparent
        if (minX == w && maxX == 0)
        {
            minX = 0;
            maxX = 0;
        }

        if (minY == h && maxY == 0)
        {
            minY = 0;
            maxY = 0;
        }

        bounds = Rect(minX, minY, maxX - minX + 1, maxY - minY + 1);

        if (!hasAlpha || !hittest)
        {
            alpha.resize(pos);
            adata = HitTestData();
        }
    }

    void ResAtlasGeneric::applyAtlas(atlas_data& ad, quint32 filter, bool clamp2edge)
    {
        if (!ad.texture)
        {
            return;
        }
        spImage mt = spImage::create();
        Rect bounds = ad.atlas.getBounds();
        qint32 w = bounds.getRight();
        qint32 h = bounds.getBottom();

        qint32 aw = w % 4;
        aw = aw ? w + 4 - aw : w;

        ImageData reg = ad.mt.lock().getRect(Rect(0, 0, aw, h));
        mt->init(reg);

        CreateTextureTask task;
        task.linearFilter = filter;
        task.clamp2edge = clamp2edge;
        task.src = mt;
        task.dest = ad.texture;
        LoadResourcesContext::get()->createTexture(task);
    }

    void ResAtlasGeneric::nextAtlas(qint32 w, qint32 h, ImageData::TextureFormat tf, atlas_data& ad)
    {
        ad.mt.init(w, h, tf);
        ad.mt.fillZero();

        ad.atlas.clean();
        ad.atlas.init(w, h);
        if (m_atlasses.size() > m_current)
        {
            ad.texture = m_atlasses[m_current].base;
        }
        else
        {
            ad.texture = IVideoDriver::instance->createTexture();

            atlas atl;
            atl.base = ad.texture;
            m_atlasses.push_back(atl);
        }
        m_current++;
    }

    void ResAtlasGeneric::_unload()
    {
    }

    void ResAtlasGeneric::_load(LoadResourcesContext*)
    {
    }

    void ResAtlasGeneric::loadAtlas(CreateResourceContext& context)
    {
        loadAtlas2(context);
    }

    void ResAtlasGeneric::loadAtlas2(CreateResourceContext& context)
    {
        m_current = 0;
        QDomElement node = context.m_walker.getNode();

        bool ok = false;
        qint32 w = node.attribute("width").toInt(&ok);
        if (!ok)
        {
            w = defaultAtlasWidth;
        }
        qint32 h = node.attribute("height").toInt(&ok);
        if (!ok)
        {
            h = defaultAtlasHeight;
        }

        loadBase(node);

        atlas_data ad;

        ImageData::TextureFormat tf = ImageData::TF_R8G8B8A8;
        QVector<spResAnim> anims;
        while (true)
        {
            XmlWalker walker = context.m_walker.next();
            if (walker.empty())
            {
                break;
            }

            QDomElement child_node = walker.getNode();

            QString name = child_node.nodeName();
            if (name != "image")
            {
                continue;
            }

            QString file = child_node.attribute("file");

            if (file.isEmpty())
            {
                createEmpty(walker, context);
                continue;
            }

            QFile fileInfo(walker.getPath("file"));
            if (!fileInfo.exists())
            {
                qCritical("Invalid item found. %s", fileInfo.fileName().toStdString().c_str());
                continue;
            }

            bool found = false;
            for (const auto& item : anims)
            {
                if (item->getResPath() == walker.getPath("file"))
                {
                    found = true;
                    break;
                }
            }
            if (found)
            {
                qCritical("Duplicate entry found. %s", fileInfo.fileName().toStdString().c_str());
                continue;
            }

            QVariant value = QVariant(child_node.attribute("trim"));
            bool trim = true;
            if (value.type() == QVariant::Type::Bool)
            {
                trim = value.toBool();
            }
            value = QVariant(child_node.attribute("extend"));
            bool extend = true;
            if (value.type() == QVariant::Type::Bool)
            {
                extend = value.toBool();
            }

            Point offset = extend ? Point(2, 2) : Point(0, 0);

            Image mt;
            ImageData im;

            qint32 columns = 0;
            qint32 rows = 0;
            qint32 frame_width = 0;
            qint32 frame_height = 0;

            QImage img(walker.getPath("file"));
            if (img.width() == 0 || img.height() == 0)
            {
                qWarning("Image. Not found %s", walker.getPath("file").toStdString().c_str());
                continue;
            }
            rows = child_node.attribute("rows").toInt();
            frame_width = child_node.attribute("frame_width").toInt();
            columns = child_node.attribute("cols").toInt();
            frame_height = child_node.attribute("frame_height").toInt();

            if (rows <= 0)
            {
                rows = 1;
            }
            if (columns <= 0)
            {
                columns = 1;
            }
            SpriteCreator::addTransparentBorder(img, columns, rows);
            if (frame_width > 0)
            {
                columns = img.width() / frame_width;
            }
            else
            {
                frame_width = img.width() / columns;
            }
            if (frame_height > 0)
            {
                rows = img.height() / frame_height;
            }
            else
            {
                frame_height = img.height() / rows;
            }
            animationFrames frames;
            qint32 frames_count = rows * columns;
            frames.reserve(frames_count);
            qint32 width = frame_width;
            qint32 height = frame_height;
            if (rows > 1 || columns > 1)
            {
                frame_width -= 1;
                frame_height -= 1;
            }

            mt.init(img, true);
            im = mt.lock();

            spResAnim ra = spResAnim::create(this);
            ra->setResPath(walker.getPath("file"));

            anims.push_back(ra);

            for (qint32 y = 0; y < rows; ++y)
            {
                for (qint32 x = 0; x < columns; ++x)
                {
                    Rect frameRect;
                    frameRect.pos = Point(x * width, y * height);
                    frameRect.size = Point(frame_width, frame_height);

                    ImageData srcImage_ = im.getRect(frameRect);


                    HitTestData adata;
                    ImageData src;
                    Rect bounds(0, 0, srcImage_.m_w, srcImage_.m_h);
                    if (trim)
                    {
                        makeAlpha(srcImage_, bounds, m_hitTestBuffer, adata, walker.getAlphaHitTest());
                    }
                    src = srcImage_.getRect(bounds);

                    Rect dest(0, 0, 0, 0);

                    if (!ad.texture)
                    {
                        nextAtlas(w, h, tf, ad);
                    }

                    bool s = ad.atlas.add(&ad.mt, src, dest, offset);
                    if (s == false)
                    {
                        applyAtlas(ad, m_linearFilter, m_clamp2edge);

                        nextAtlas(w, h, tf, ad);
                        s = ad.atlas.add(&ad.mt, src, dest, offset);
                        Q_ASSERT(s);
                    }

                    //extend = false;
                    if (extend)
                    {
                        //duplicate image edges
                        Image& mt = ad.mt;
                        ImageData tmp;

                        if (bounds.getY() == 0 && dest.pos.y != 0)
                        {
                            tmp = mt.lock(Rect(dest.pos.x, dest.pos.y - 1, src.m_w, 1));
                            operations::copy(src.getRect(Rect(0, 0, src.m_w, 1)), tmp);
                        }

                        if (bounds.getHeight() == im.m_h && dest.getBottom() != mt.getHeight())
                        {
                            tmp = mt.lock(Rect(dest.pos.x, dest.pos.y + src.m_h, src.m_w, 1));
                            operations::copy(src.getRect(Rect(0, src.m_h - 1, src.m_w, 1)), tmp);
                        }

                        if (bounds.getX() == 0 && dest.pos.x != 0)
                        {
                            tmp = mt.lock(Rect(dest.pos.x - 1, dest.pos.y, 1, src.m_h));
                            operations::copy(src.getRect(Rect(0, 0, 1, src.m_h)), tmp);
                        }

                        if (bounds.getWidth() == im.m_w && dest.getRight() != mt.getWidth())
                        {
                            tmp = mt.lock(Rect(dest.pos.x + src.m_w, dest.pos.y, 1, src.m_h));
                            operations::copy(src.getRect(Rect(src.m_w - 1, 0, 1, src.m_h)), tmp);
                        }
                    }
                    float iw = 1.0f;
                    float ih = 1.0f;

                    RectF srcRect(dest.pos.x * iw, dest.pos.y * ih, dest.size.x * iw, dest.size.y * ih);

                    Vector2 sizeScaled = Vector2((float)dest.size.x, (float)dest.size.y) * walker.getScaleFactor();
                    RectF destRect(bounds.pos.cast<Vector2>(), sizeScaled);

                    AnimationFrame frame;
                    Diffuse df;
                    df.base = ad.texture;

                    Vector2 fsize = Vector2((float)frame_width, (float)frame_height) * walker.getScaleFactor();
                    frame.init2(ra.get(), x, y, df,
                                srcRect, destRect, fsize);

                    frame.setHitTestData(adata);

                    frames.push_back(frame);
                }
            }

            init_resAnim(ra, file, child_node);

            ra->init(frames, columns, walker.getScaleFactor(), 1.0f / walker.getScaleFactor());
            ra->setParent(this);
            context.m_resources->add(ra, context.m_options->m_shortenIDS);
        }

        applyAtlas(ad, m_linearFilter, m_clamp2edge);

        for (QVector<spResAnim>::iterator i = anims.begin(); i != anims.end(); ++i)
        {
            spResAnim rs = *i;
            qint32 num = rs->getTotalFrames();

            for (qint32 n = 0; n < num; ++n)
            {
                AnimationFrame& frame = const_cast<AnimationFrame&>(rs->getFrame(n));

                float iw = 1.0f / frame.getDiffuse().base->getWidth();
                float ih = 1.0f / frame.getDiffuse().base->getHeight();

                RectF rect = frame.getSrcRect();
                rect.pos.x *= iw;
                rect.pos.y *= ih;
                rect.size.x *= iw;
                rect.size.y *= ih;
                frame.setSrcRect(rect);

                HitTestData ad = frame.getHitTestData();
                if (ad.pitch)
                {
                    ad.data = &m_hitTestBuffer[reinterpret_cast<size_t>(ad.data)];
                    frame.setHitTestData(ad);
                }
            }
        }
    }

}
