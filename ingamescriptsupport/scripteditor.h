#ifndef SCRIPTEDITOR_H
#define SCRIPTEDITOR_H

#include <QObject>


#include "3rd_party/oxygine-framework/oxygine-framework.h"

#include "objects/base/panel.h"

#include "objects/base/checkbox.h"
#include "objects/base/dropdownmenu.h"

#include "ingamescriptsupport/scriptdata.h"

class ScriptEditor;
typedef oxygine::intrusive_ptr<ScriptEditor> spScriptEditor;

class ScriptEditor : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit ScriptEditor();
    virtual ~ScriptEditor() = default;
signals:
    /**
     * @brief sigSaveScript
     */
    void sigSaveScript();
    /**
     * @brief sigLoadScript
     */
    void sigLoadScript();
    /**
     * @brief sigFinished
     */
    void sigFinished();
    /**
     * @brief sigAddCondition
     */
    void sigAddCondition();
    /**
     * @brief sigAddEvent
     */
    void sigAddEvent();
    /**
     * @brief sigUpdateConditions
     */
    void sigUpdateConditions();
    /**
     * @brief sigUpdateEvents
     */
    void sigUpdateEvents();
    /**
     * @brief sigShowEditCondition
     * @param pCondition
     */
    void sigShowEditCondition(spScriptCondition pCondition);
    /**
     * @brief sigShowEditEvent
     * @param pEvent
     */
    void sigShowEditEvent(spScriptEvent pEvent);
    /**
     * @brief sigShowExitBox
     */
    void sigShowExitBox();
    /**
     * @brief sigDuplicateEvent
     */
    void sigDuplicateEvent(spScriptEvent pEvent);
    /**
     * @brief sigDuplicateCondition
     */
    void sigDuplicateCondition();
public slots:
    /**
     * @brief saveScript
     */
    void showSaveScript();
    /**
     * @brief loadScript
     */
    void showLoadScript();
    /**
     * @brief saveScript
     */
    void saveScript(QString filename);
    /**
     * @brief loadScript
     */
    void loadScript(QString filename);
    /**
     * @brief changeImmediateStart
     */
    void changeImmediateStart();
    /**
     * @brief updateConditios
     */
    void updateConditios();
    /**
     * @brief addConditionEntry
     * @param pCondition
     * @param y
     */
    void addConditionEntry(spScriptCondition pCondition, qint32& y);
    /**
     * @brief addCondition
     */
    void addCondition();
    /**
     * @brief addEvent
     */
    void addEvent();
    /**
     * @brief updateEvents
     */
    void updateEvents();
    /**
     * @brief addEventEntry
     * @param pEvent
     * @param y
     */
    void addEventEntry(spScriptEvent pEvent, qint32& y);
    /**
     * @brief showEditCondition
     * @param pCondition
     */
    void showEditCondition(spScriptCondition pCondition);
    /**
     * @brief showEditEvent
     * @param pEvent
     */
    void showEditEvent(spScriptEvent pEvent);
    /**
     * @brief showExitBox
     */
    void showExitBox();
    /**
     * @brief exitEditor
     */
    void exitEditor();
    /**
     * @brief duplicateEvent
     */
    void duplicateEvent(spScriptEvent pEvent);
    /**
     * @brief duplicateCondition
     */
    void duplicateCondition();
    /**
     * @brief addConditionToData
     * @param pCondition
     */
    void addConditionToData(spScriptCondition pCondition);
private:
    spPanel m_ConditionPanel;
    spPanel m_EventPanel;
    spCheckbox m_ImmediateStart;
    spScriptData m_Data;
    spDropDownmenu m_Conditions;
    spDropDownmenu m_Events;
    spScriptCondition m_CurrentCondition;
    QVector<oxygine::spBox9Sprite> m_ConditionBoxes;
};

#endif // SCRIPTEDITOR_H
