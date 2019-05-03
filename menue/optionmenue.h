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
    void sigShowGameplayAndKeys();
    void sigChangeScreenSize(qint32 width, qint32 heigth);
public slots:
    void exitMenue();
    void showMods();
    void restart();
    void showSettings();
    void showGameplayAndKeys();
    void changeScreenMode(qint32 mode);
    void changeScreenSize(qint32 width, qint32 heigth);
private:
    spPanel m_pOptions;
    bool restartNeeded{false};
};

#endif // OPTIONMENUE_H
