#ifndef DIALOGVICTORYCONDITIONS_H
#define DIALOGVICTORYCONDITIONS_H

#include <QObject>
#include <QVector>

#include "oxygine-framework.h"

#include "objects/checkbox.h"

class DialogVictoryConditions;
typedef oxygine::intrusive_ptr<DialogVictoryConditions> spDialogVictoryConditions;

class DialogVictoryConditions : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit DialogVictoryConditions();
    virtual ~DialogVictoryConditions() = default;

signals:

public slots:

private:
    oxygine::spButton m_OkButton;
};

#endif // DIALOGVICTORYCONDITIONS_H
