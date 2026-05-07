#ifndef DIALOGMODSYNCPROGRESS_H
#define DIALOGMODSYNCPROGRESS_H

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

    void setProgress(qint32 stagedMods, qint64 receivedBytes);
signals:
    void sigCancel();
public slots:
    void remove();
private:
    qint32 m_totalMods{0};
    qint32 m_barWidth{0};
    oxygine::spColorRectSprite m_BarBackground;
    oxygine::spColorRectSprite m_BarFill;
    oxygine::spTextField m_Header;
    oxygine::spTextField m_Detail;
    oxygine::spButton m_CancelButton;
};

#endif
