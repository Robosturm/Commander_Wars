#ifndef QRCODEACTOR_H
#define QRCODEACTOR_H

#include <QObject>

#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Sprite.h"
#include "3rd_party/oxygine-framework/oxygine/res/SingleResAnim.h"

class QrCodeActor;
using spQrCodeActor = std::shared_ptr<QrCodeActor>;

/**
 * @brief The QrCodeActor class renders a runtime-generated QR code (e.g. an otpauth://
 * url for 2 factor authentication enrollment) as a sprite child inside an UiFactory
 * created dialog. The QR code is created with the vendored Nayuki qrcodegen library,
 * so no additional system dependency is needed.
 */
class QrCodeActor final : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    /**
     * @brief PIXEL_PER_MODULE scale factor of one QR code module in pixels
     */
    static constexpr qint32 PIXEL_PER_MODULE = 4;
    /**
     * @brief QUIET_ZONE_MODULES empty modules rendered around the QR code (required by the standard)
     */
    static constexpr qint32 QUIET_ZONE_MODULES = 4;

    explicit QrCodeActor();
    virtual ~QrCodeActor() = default;
    /**
     * @brief setQrData generates and displays a QR code for the given data
     * @param data payload of the QR code (e.g. an otpauth:// url)
     */
    Q_INVOKABLE void setQrData(const QString & data);
    /**
     * @brief getQrSize pixel size of the generated QR code image
     * @return width = height of the current QR code, 0 if no data is set
     */
    Q_INVOKABLE qint32 getQrSize() const;
private:
    oxygine::spSprite m_pSprite;
    oxygine::spSingleResAnim m_pResAnim;
};

#endif // QRCODEACTOR_H
