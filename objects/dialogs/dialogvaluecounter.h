#ifndef DIALOGVALUECOUNTER_H
#define DIALOGVALUECOUNTER_H

#include <QObject>
#include <QTimer>


#include "3rd_party/oxygine-framework/oxygine/actor/Button.h"
#include "objects/base/label.h"

class DialogValueCounter;
using spDialogValueCounter = std::shared_ptr<DialogValueCounter>;

class DialogValueCounter final : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit DialogValueCounter(qint32 totalPoints, qint32 pointsToAdd);
   virtual ~DialogValueCounter() = default;
signals:
    void sigFinished();
private slots:
    void updateData();
    void remove();
private:
    spLabel m_pointsAddedLabel;
    spLabel m_totalPointsLabel;
    qint32 m_totalPoints{0};
    qint32 m_pointsToAdd{0};
    oxygine::spButton m_OkButton;
    QTimer m_updateTimer;
};

#endif // DIALOGVALUECOUNTER_H
