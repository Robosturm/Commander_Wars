#ifndef RULESELECTION_H
#define RULESELECTION_H

#include <QObject>

#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"

#include "objects/base/multislider.h"
#include "objects/base/textbox.h"

#include "coreengine/LUPDATE_MACROS.h"

class GameMap;
class RuleSelection;
using spRuleSelection = oxygine::intrusive_ptr<RuleSelection>;

class RuleSelection : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    ENUM_CLASS Mode
    {
        Editor,
        Singleplayer,
        Multiplayer,
    };
    explicit RuleSelection(GameMap* pMap, qint32 width, Mode mode, bool enabled = true);
    virtual ~RuleSelection() = default;
    void showRuleSelection(bool advanced = false);
    void confirmRuleSelectionSetup();
    Mode getMode() const;

signals:
    void sigShowCOBannlist();
    void sigShowActionBannlist();
    void sigShowPerkBannlist();
    void sigShowSelectScript();
    void sigSizeChanged();
public slots:
    // slots for changing the rules
    GameMap *getMap() const;
    void startWeatherChanged(qint32 value);
    void weatherChancesChanged();
    void showCOBannlist();
    void showPerkBannlist();
    void showActionBannlist();
    void showSelectScript();
    void scriptFileChanged(QString file);
private:
    void addCustomGamerules(qint32 & y);
private:
    spMultislider m_pWeatherSlider;
    spTextbox m_MapScriptFile;
    Mode m_mode;
    bool m_ruleChangeEabled{true};
    GameMap* m_pMap{nullptr};
};

#endif // RULESELECTION_H
