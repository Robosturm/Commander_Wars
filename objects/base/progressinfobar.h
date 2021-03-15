#ifndef PROGRESSINFOBAR_H
#define PROGRESSINFOBAR_H

#include <QObject>

#include "objects/base/label.h"


#include "3rd_party/oxygine-framework/oxygine-framework.h"

class ProgressInfoBar;
typedef oxygine::intrusive_ptr<ProgressInfoBar> spProgressInfoBar;

class ProgressInfoBar : public Tooltip
{
    Q_OBJECT
public:
    explicit ProgressInfoBar(qint32 width, qint32 heigth, QString text, float progress);

public slots:
    void setText(QString text);
    void setProgress(qint32 progress);

private:
    oxygine::spColorRectSprite m_Background;
    oxygine::spColorRectSprite m_Progressbar;
    spLabel m_Label;
};

#endif // PROGRESSINFOBAR_H
