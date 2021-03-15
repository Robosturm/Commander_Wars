#ifndef DIALOGMODIFYUNIT_H
#define DIALOGMODIFYUNIT_H

#include <QObject>


#include "3rd_party/oxygine-framework/oxygine-framework.h"

#include "objects/base/checkbox.h"

#include "objects/base/panel.h"

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
    void sigUpdateData();
    void sigLoadUnit(QString unitID, qint32 index);
public slots:

protected slots:
    void updateData();
    void loadUnit(QString unitID, qint32 index);
    void addLoadLoopPoints(qint32& y, qint32 sliderOffset);
private:
    void addLoadUnit(qint32 index, qint32 sliderOffset, qint32& y);
    spPanel m_pPanel;
    Unit* m_pUnit{nullptr};
    oxygine::spButton m_OkButton;
};

#endif // DIALOGMODIFYUNIT_H
