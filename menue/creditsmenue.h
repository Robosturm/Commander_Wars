#ifndef CREDITSMENUE_H
#define CREDITSMENUE_H

#include "3rd_party/oxygine-framework/oxygine-framework.h"
#include "menue/basemenu.h"

#include <QVector>
#include <QElapsedTimer>

#include <QObject>

class CreditsMenue;
using spCreditsMenue = oxygine::intrusive_ptr<CreditsMenue>;

class CreditsMenue : public Basemenu
{
    Q_OBJECT
public:
    explicit CreditsMenue();

    virtual void doUpdate(const oxygine::UpdateState&) override;
signals:
    void sigExitMenue();

public slots:
    void exitMenue();

private:
    QElapsedTimer m_speedTimer;
    QVector<QString> m_Headlines;
    QVector<QVector<QString>> m_Authors;
    qint32 m_creditsHeigth{100};
    oxygine::spActor m_creditsActor;
};

#endif // CREDITSMENUE_H
