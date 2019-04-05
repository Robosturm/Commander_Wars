#ifndef OPTIONMENUE_H
#define OPTIONMENUE_H

#include <QObject>

#include "oxygine-framework.h"

#include "objects/panel.h"

class OptionMenue : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit OptionMenue();

signals:
    void sigExitMenue();
    void sigShowMods();
public slots:
    void exitMenue();
    void showMods();

private:
    spPanel m_pOptions;
};

#endif // OPTIONMENUE_H
