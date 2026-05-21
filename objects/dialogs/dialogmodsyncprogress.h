#ifndef DIALOGMODSYNCPROGRESS_H
#define DIALOGMODSYNCPROGRESS_H

#include <QElapsedTimer>
#include <QObject>

#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Button.h"
#include "3rd_party/oxygine-framework/oxygine/actor/ColorRectSprite.h"
#include "3rd_party/oxygine-framework/oxygine/actor/TextField.h"

class DialogModSyncProgress;
using spDialogModSyncProgress = std::shared_ptr<DialogModSyncProgress>;

class DialogModSyncProgress final : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit DialogModSyncProgress(qint32 totalMods);
    virtual ~DialogModSyncProgress() = default;

    // 0 means unknown; once set the field refuses downward revision so a late or hostile follow-up can't shrink the bar.
    void setExpectedTotalBytes(qint64 expectedUncompressed);
    void setProgress(qint32 stagedMods, qint64 receivedCompressed, qint64 receivedUncompressed);
signals:
    void sigCancel();
public slots:
    void remove();
private:
    static QString formatBytes(qint64 bytes);
    static QString formatRate(qint64 bytesPerSecond);
    static QString formatEta(qint64 seconds);

    qint32 m_totalMods{0};
    qint32 m_barWidth{0};
    qint64 m_expectedTotalBytes{0};
    qint64 m_lastReceivedCompressed{0};
    qint64 m_lastReceivedUncompressed{0};
    qint64 m_lastSampleMs{0};
    // Compressed rate drives the displayed network throughput; uncompressed rate drives ETA so units match remaining-uncompressed.
    double m_smoothedRateBytesPerSec{0.0};
    double m_smoothedUncompressedRateBytesPerSec{0.0};
    float m_lastDisplayedFraction{0.0f};
    QElapsedTimer m_timer;
    oxygine::spColorRectSprite m_BarBackground;
    oxygine::spColorRectSprite m_BarFill;
    oxygine::spTextField m_Header;
    oxygine::spTextField m_Detail;
    oxygine::spButton m_CancelButton;
};

#endif
