#ifndef COINFODIALOG_H
#define COINFODIALOG_H

#include <QObject>

#include <QVector>

#include "game/smartCO.h"

#include "game/player.h"

#include "game/co.h"

#include "oxygine-framework.h"

#include "panel.h"

class COInfoDialog;
typedef oxygine::intrusive_ptr<COInfoDialog> spCOInfoDialog;

class COInfoDialog : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit COInfoDialog(spCO pCO, spPlayer pPlayer,
                          std::function<void (spCO& m_CurrrentCO, spPlayer& pPlayer, qint32 direction)> stepFunction,
                          bool ingame);
    virtual ~COInfoDialog() = default;


    void showCO();
    void createStrengthBar(oxygine::spActor pActor, qint32 bonus, qint32 y);
signals:
    void next();
    void quit();
    void back();
public slots:
    void slotNext();
    void slotBack();
private:
    oxygine::spButton m_NextButton;
    oxygine::spButton m_QuitButton;
    oxygine::spButton m_BackButton;
    spPanel m_pPanel;

    oxygine::spSprite m_pCurrentCO;

    oxygine::spTextField m_COName;
    oxygine::spTextField m_COBio;

    oxygine::spBox9Sprite m_HitSprite;
    oxygine::spTextField m_HitText;

    oxygine::spSprite m_MissSprite;
    oxygine::spTextField m_MissText;

    oxygine::spSprite m_InfoSprite;
    oxygine::spTextField m_InfoText;

    oxygine::spSprite m_PowerSprite;
    oxygine::spTextField m_Powername;
    oxygine::spTextField m_PowerDesc;

    oxygine::spSprite m_SuperPowerSprite;
    oxygine::spTextField m_SuperPowername;
    oxygine::spTextField m_SuperPowerDesc;

    QVector<oxygine::spActor> m_UnitDataActors;

    spCO m_CurrentCO;
    spPlayer m_pCurrentPlayer{nullptr};

    std::function<void (spCO& m_CurrrentCO, spPlayer& pPlayer, qint32 direction)> m_StepFunction;

    bool m_Ingame{false};
};

#endif // COINFODIALOG_H
