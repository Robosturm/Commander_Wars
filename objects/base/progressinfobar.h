#ifndef PROGRESSINFOBAR_H
#define PROGRESSINFOBAR_H

#include <QObject>

#include "3rd_party/oxygine-framework/oxygine/actor/ColorRectSprite.h"

#include "objects/base/label.h"

class ProgressInfoBar;
using spProgressInfoBar = oxygine::intrusive_ptr<ProgressInfoBar>;

class ProgressInfoBar final : public Tooltip
{
    Q_OBJECT
public:
    explicit ProgressInfoBar(qint32 width, qint32 heigth, QString text, float progress);
   virtual ~ProgressInfoBar() = default;
public slots:
    void setText(QString text);
    void setProgress(qint32 progress);

private:
    oxygine::spColorRectSprite m_Background;
    oxygine::spColorRectSprite m_Progressbar;
    spLabel m_Label;
};

#endif // PROGRESSINFOBAR_H
