#ifndef RULESELECTION_H
#define RULESELECTION_H

#include "ui_reader/createdgui.h"
#include "coreengine/LUPDATE_MACROS.h"

class GameMap;
class RuleSelection;
using spRuleSelection = oxygine::intrusive_ptr<RuleSelection>;

class RuleSelection final : public CreatedGui
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
    ~RuleSelection() = default;
    void confirmRuleSelectionSetup();
    Mode getMode() const;

signals:
    void sigSizeChanged();
    void sigShowRuleSelection();
public slots:
    bool getRuleChangeEabled() const;
    bool getShowAdvanced() const;
    void setShowAdvanced(bool newShowAdvanced);
    void showRuleSelection();
    bool getIsMultiplayerMode();
    bool getIsEditorMode();
    // slots for changing the rules
    GameMap *getMap() const;
    void startWeatherChanged(qint32 value);
    void weatherChancesChanged(const QString & id);
    void showCOBannlist();
    void showPerkBannlist();
    void showActionBannlist();
    void showSelectScript();
    void scriptFileChanged(const QString & file);

private:    
    Mode m_mode;
    bool m_ruleChangeEabled{true};
    bool m_showAdvanced{false};
    GameMap* m_pMap{nullptr};
};

#endif // RULESELECTION_H
