#ifndef VICTORYRULEPOPUP_H
#define VICTORYRULEPOPUP_H

#include "objects/base/closeablepopup.h"

class VictoryRulePopup;
using spVictoryRulePopup = oxygine::intrusive_ptr<VictoryRulePopup>;

class VictoryRulePopup : public CloseablePopUp
{
public:
    VictoryRulePopup(QString rule, qint32 width, qint32 heigth);
    virtual ~VictoryRulePopup();
    static bool exists(QString rule)
    {
        return m_popUps.contains(rule);
    };

public slots:
    void updateInfo();
private:
    QString m_rule;
    static QStringList m_popUps;
};

#endif // VICTORYRULEPOPUP_H
