#ifndef CREDITSMENUE_H
#define CREDITSMENUE_H

#include <QObject>


#include "3rd_party/oxygine-framework/oxygine-framework.h"

#include <QObject>
#include <QVector>
#include <QElapsedTimer>

class CreditsMenue : public QObject, public oxygine::Actor
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
    QElapsedTimer speedTimer;
    QVector<QString> m_Headlines;
    QVector<QVector<QString>> m_Authors;
    qint32 m_creditsHeigth{100};
    oxygine::spActor creditsActor;
};

#endif // CREDITSMENUE_H
