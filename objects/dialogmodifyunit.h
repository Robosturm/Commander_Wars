#ifndef DIALOGMODIFYUNIT_H
#define DIALOGMODIFYUNIT_H

#include <QObject>

#include "oxygine-framework.h"

#include "objects/checkbox.h"

class Unit;

class DialogModifyUnit;
typedef oxygine::intrusive_ptr<DialogModifyUnit> spDialogModifyUnit;

class DialogModifyUnit : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit DialogModifyUnit(Unit* pUnit);
    virtual ~DialogModifyUnit() = default;
signals:
    void sigFinished();
public slots:
private:
    Unit* m_pUnit{nullptr};
    oxygine::spButton m_OkButton;
};

#endif // DIALOGMODIFYUNIT_H
