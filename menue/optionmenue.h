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
    void sigShowSettings();
public slots:
    void exitMenue();
    void showMods();
    void restart();
    void showSettings();
private:
    spPanel m_pOptions;
    bool restartNeeded{false};
};

#endif // OPTIONMENUE_H
