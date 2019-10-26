#ifndef OPTIONMENUE_H
#define OPTIONMENUE_H

#include <QObject>

#include <qvector.h>

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

    void reloadSettings();
private:
    spPanel m_pOptions;

    spPanel m_pMods;
    spPanel m_pModDescription;
    oxygine::spTextField m_ModDescriptionText;
    QVector<oxygine::spActor> m_ModBoxes;

    bool restartNeeded{false};
};

#endif // OPTIONMENUE_H
