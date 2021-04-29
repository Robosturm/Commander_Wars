#ifndef CLOSEABLEPOPUP_H
#define CLOSEABLEPOPUP_H

#include <QObject>

#include "3rd_party/oxygine-framework/oxygine-framework.h"
#include "objects/base/panel.h"

class CloseablePopUp;
using spCloseablePopUp = oxygine::intrusive_ptr<CloseablePopUp>;

class CloseablePopUp : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit CloseablePopUp(qint32 width, qint32 heigth);

    void addItem(oxygine::spActor pActor);
    void removeItem(oxygine::spActor pActor);
    void setContentHeigth(qint32 heigth);
    void setContentWidth(qint32 width);

    qint32 getContentHeigth();
    qint32 getContentWidth();

    void clearContent();
signals:
    void sigClosed();
public slots:
    void setLocked(bool locked);
protected slots:
    void close();
private:
     oxygine::Draggable m_drag;
     spPanel m_pPanel;
};

#endif // CLOSEABLEPOPUP_H
