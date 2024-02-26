#ifndef RULESELECTION_H
#define RULESELECTION_H

#include "ui_reader/createdgui.h"

class GameMap;
class RuleSelection;
using spRuleSelection = std::shared_ptr<RuleSelection>;

class RuleSelection final : public CreatedGui
{
    Q_OBJECT
public:
    enum class Mode
    {
        Editor,
        Singleplayer,
        Multiplayer,
        MultiplayerOnlyGateway,
    };
    explicit RuleSelection(GameMap* pMap, qint32 width, Mode mode, bool enabled = true);
    virtual ~RuleSelection() = default;
    Mode getMode() const;
    Q_INVOKABLE bool getRuleChangeEabled() const;
    Q_INVOKABLE bool getShowAdvanced() const;
    Q_INVOKABLE void setShowAdvanced(bool newShowAdvanced);
    Q_INVOKABLE void showRuleSelection();
    Q_INVOKABLE bool getIsMultiplayerMode();
    Q_INVOKABLE bool getIsMultiplayerOnlyGatewayMode();
    Q_INVOKABLE bool getIsEditorMode();
    // slots for changing the rules
    Q_INVOKABLE GameMap *getMap() const;
    Q_INVOKABLE void startWeatherChanged(qint32 value);
    Q_INVOKABLE void weatherChancesChanged(const QString id);
    Q_INVOKABLE void showCOBannlist();
    Q_INVOKABLE void showPerkBannlist();
    Q_INVOKABLE void showActionBannlist();
    Q_INVOKABLE void showSelectScript();
    Q_INVOKABLE void scriptFileChanged(const QString file);

signals:
    void sigSizeChanged();
    void sigShowRuleSelection();

private:    
    Mode m_mode;
    bool m_ruleChangeEabled{true};
    bool m_showAdvanced{false};
    GameMap* m_pMap{nullptr};
};

#endif // RULESELECTION_H
