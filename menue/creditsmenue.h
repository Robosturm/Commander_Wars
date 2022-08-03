#ifndef CREDITSMENUE_H
#define CREDITSMENUE_H

#include <QVector>
#include <QElapsedTimer>
#include <QObject>

#include "menue/basemenu.h"

class CreditsMenue;
using spCreditsMenue = oxygine::intrusive_ptr<CreditsMenue>;

class CreditsMenue : public Basemenu
{
    Q_OBJECT
public:
    explicit CreditsMenue();
    virtual ~CreditsMenue() = default;
    virtual void doUpdate(const oxygine::UpdateState&) override;
signals:
    void sigExitMenue();
public slots:
    void exitMenue();
protected slots:
    virtual void onEnter() override;
private:
    QElapsedTimer m_speedTimer;
    QStringList m_Headlines;
    QVector<QStringList> m_Authors;
    qint32 m_creditsHeigth{100};
    oxygine::spActor m_creditsActor;
};

Q_DECLARE_INTERFACE(CreditsMenue, "CreditsMenue");

#endif // CREDITSMENUE_H
