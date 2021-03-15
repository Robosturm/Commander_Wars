#ifndef COINFODIALOG_H
#define COINFODIALOG_H

#include <QObject>

#include <QVector>

#include "game/player.h"

#include "game/co.h"


#include "3rd_party/oxygine-framework/oxygine-framework.h"

#include "objects/base/panel.h"

#include "objects/coinfoactor.h"

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

    /**
     * @brief showCO
     */
    void showCO();
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

    spCOInfoActor m_COInfo;

    spCO m_CurrentCO;
    spPlayer m_pCurrentPlayer{nullptr};

    std::function<void (spCO& m_CurrrentCO, spPlayer& pPlayer, qint32 direction)> m_StepFunction;

    bool m_Ingame{false};
};

#endif // COINFODIALOG_H
