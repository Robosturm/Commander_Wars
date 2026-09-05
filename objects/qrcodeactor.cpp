#include "objects/qrcodeactor.h"

#include <QImage>

#include "3rd_party/qrcodegen/qrcodegen.hpp"

#include "coreengine/gameconsole.h"
#include "coreengine/interpreter.h"
#include "coreengine/memorymanagement.h"

QrCodeActor::QrCodeActor()
{
#ifdef GRAPHICSUPPORT
    setObjectName("QrCodeActor");
#endif
    Interpreter::setCppOwnerShip(this);
    m_pSprite = MemoryManagement::create<oxygine::Sprite>();
    m_pSprite->setVisible(false);
    addChild(m_pSprite);
}

void QrCodeActor::setQrData(const QString & data)
{
    if (data.isEmpty())
    {
        m_pSprite->setVisible(false);
        return;
    }
    // medium error correction: enough payload for otpauth urls and robust scanning
    const qrcodegen::QrCode qrCode = qrcodegen::QrCode::encodeText(data.toUtf8().constData(), qrcodegen::QrCode::Ecc::MEDIUM);
    const qint32 modules = qrCode.getSize();
    const qint32 imageSize = (modules + 2 * QUIET_ZONE_MODULES) * PIXEL_PER_MODULE;
    QImage image(imageSize, imageSize, QImage::Format_RGBA8888);
    image.fill(Qt::white);
    for (qint32 y = 0; y < modules; ++y)
    {
        for (qint32 x = 0; x < modules; ++x)
        {
            if (qrCode.getModule(x, y))
            {
                const qint32 startX = (x + QUIET_ZONE_MODULES) * PIXEL_PER_MODULE;
                const qint32 startY = (y + QUIET_ZONE_MODULES) * PIXEL_PER_MODULE;
                for (qint32 dy = 0; dy < PIXEL_PER_MODULE; ++dy)
                {
                    uchar* scanline = image.scanLine(startY + dy);
                    for (qint32 dx = 0; dx < PIXEL_PER_MODULE; ++dx)
                    {
                        uchar* pixel = scanline + (startX + dx) * 4;
                        pixel[0] = 0;
                        pixel[1] = 0;
                        pixel[2] = 0;
                        pixel[3] = 255;
                    }
                }
            }
        }
    }
    m_pResAnim = MemoryManagement::create<oxygine::SingleResAnim>();
    // nearest filter keeps the code crisp and scannable
    m_pResAnim->init(image, 1, 1, 1.0f, true, 0x2600 /*GL_NEAREST*/);
    m_pSprite->setResAnim(m_pResAnim.get());
    m_pSprite->setSize(imageSize, imageSize);
    m_pSprite->setVisible(true);
    setSize(imageSize, imageSize);
}

qint32 QrCodeActor::getQrSize() const
{
    if (m_pSprite->getVisible() && m_pResAnim.get() != nullptr)
    {
        return static_cast<qint32>(m_pSprite->getWidth());
    }
    return 0;
}
