#ifndef OPTIONMENUE_H
#define OPTIONMENUE_H

#include <QObject>

#include <qvector.h>

#include "oxygine-framework.h"

#include "objects/base/panel.h"

#include "objects/gameplayandkeys.h"

class OptionMenue : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit OptionMenue();
    virtual ~OptionMenue();
signals:
    void sigExitMenue();
    void sigShowMods();
    void sigShowSettings();
    void sigShowGameplayAndKeys();
    void sigChangeScreenSize(qint32 width, qint32 heigth);
    void sigReloadSettings();
public slots:
    void exitMenue();
    void showMods();
    void restart();
    void showSettings();
    void showGameplayAndKeys();
    void selectMods(qint32 item);
    void reloadSettings();
private:
    spGameplayAndKeys m_pGameplayAndKeys;

    spPanel m_pOptions;
    spPanel m_pMods;
    spPanel m_pModDescription;
    oxygine::spActor m_ModSelector;
    oxygine::spTextField m_ModDescriptionText;
    QVector<oxygine::spActor> m_ModBoxes;

    QStringList languages;

    bool restartNeeded{false};
};

#endif // OPTIONMENUE_H
