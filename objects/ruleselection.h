#ifndef RULESELECTION_H
#define RULESELECTION_H

#include <QObject>

#include "3rd_party/oxygine-framework/oxygine-framework.h"

#include "objects/base/multislider.h"
#include "objects/base/textbox.h"

#include "coreengine/LUPDATE_MACROS.h"

class RuleSelection;
typedef oxygine::intrusive_ptr<RuleSelection> spRuleSelection;

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
    explicit RuleSelection(qint32 width, Mode mode, bool enabled = true);
    virtual ~RuleSelection();
    void showRuleSelection();
    void confirmRuleSelectionSetup();
    Mode getMode() const;

signals:
    void sigShowCOBannlist();
    void sigShowActionBannlist();
    void sigShowPerkBannlist();
    void sigShowSelectScript();
public slots:
    // slots for changing the rules
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
};

#endif // RULESELECTION_H
