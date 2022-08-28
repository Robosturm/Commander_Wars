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
    CustomDialog(const QString & jsName, const QString & uiXml, Basemenu* pBaseMenu, const QString & confirmText = "");
    ~CustomDialog();
signals:
    void sigFinished();
public slots:
    /**
     * @brief loadXmlFile
     * @param uiXml
     */
    void loadXmlFile(const QString& uiXml);
    /**
     * @brief getBaseMenu
     * @return
     */
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
     * @brief showMessageBox
     */
    void showMessageBox(QString text, bool withCancel = false, QString confirmText = tr("Ok"), QString cancelText = tr("Cancel"));
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
