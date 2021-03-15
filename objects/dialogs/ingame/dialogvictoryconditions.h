#ifndef DIALOGVICTORYCONDITIONS_H
#define DIALOGVICTORYCONDITIONS_H

#include <QObject>
#include <QVector>


#include "3rd_party/oxygine-framework/oxygine-framework.h"

#include "objects/base/checkbox.h"

class DialogVictoryConditions;
typedef oxygine::intrusive_ptr<DialogVictoryConditions> spDialogVictoryConditions;

class DialogVictoryConditions : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit DialogVictoryConditions();
    virtual ~DialogVictoryConditions() = default;

signals:
    void sigFinished();
    void sigShowPopup(QString rule);
protected slots:
    void showPopup(QString rule);
private:
    oxygine::spButton m_OkButton;
};

#endif // DIALOGVICTORYCONDITIONS_H
