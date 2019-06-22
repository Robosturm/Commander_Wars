#ifndef SCRIPTEDITOR_H
#define SCRIPTEDITOR_H

#include <QObject>

#include "oxygine-framework.h"

#include "objects/panel.h"

#include "objects/checkbox.h"
#include "objects/dropdownmenu.h"

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
     * @brief selectCondition
     * @param index
     */
    void selectCondition(qint32 index);
    /**
     * @brief addCondition
     */
    void addCondition();
    /**
     * @brief addEvent
     */
    void addEvent();
private:
    spPanel m_ConditionPanel;
    spPanel m_EventPanel;
    spCheckbox m_ImmediateStart;
    spScriptData m_Data;
    spDropDownmenu m_Conditions;
    spDropDownmenu m_Events;
};

#endif // SCRIPTEDITOR_H
