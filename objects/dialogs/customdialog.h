#pragma once

#include "ui_reader/createdgui.h"
#include "menue/basemenu.h"
#include "coreengine/scriptvariables.h"

class CustomDialog;
using spCustomDialog = oxygine::intrusive_ptr<CustomDialog>;

class CustomDialog final : public CreatedGui
{
    Q_OBJECT
public:
    CustomDialog(const QString & jsName, const QString & uiXml, Basemenu* pBaseMenu);
    ~CustomDialog();
signals:
    void sigFinished();
public slots:
    Basemenu* getBaseMenu();
    /**
     * @brief getVariables
     * @return
     */
    inline ScriptVariables* getVariables()
    {
        return &m_Variables;
    }
    void exit();
    /**
     * @brief createDialog
     * @param jsName
     * @param uiXml
     * @param pBaseMenu
     */
    void createDialog(const QString & jsName, const QString & uiXml, Basemenu* pBaseMenu);
private slots:
    void remove();

private:
    QString m_jsName;
    Basemenu* m_pBaseMenu{nullptr};
    ScriptVariables m_Variables;
};

Q_DECLARE_INTERFACE(CustomDialog, "CustomDialog");
