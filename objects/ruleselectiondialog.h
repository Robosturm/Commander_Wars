#ifndef RULESELECTIONDIALOG_H
#define RULESELECTIONDIALOG_H

#include <QObject>

#include "oxygine-framework.h"

#include "objects/ruleselection.h"

class RuleSelectionDialog;
typedef oxygine::intrusive_ptr<RuleSelectionDialog> spRuleSelectionDialog;

class RuleSelectionDialog : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit RuleSelectionDialog();
    virtual ~RuleSelectionDialog() = default;
signals:
    void sigRulesChanged();
private:
    oxygine::spButton m_OkButton;
    spRuleSelection m_pRuleSelection;
};

#endif // RULESELECTIONDIALOG_H
