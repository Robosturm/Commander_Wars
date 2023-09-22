#pragma once

#include "ui_reader/createdgui.h"
#include "menue/basemenu.h"
#include "coreengine/scriptvariables.h"

class CustomDialog;
using spCustomDialog = std::shared_ptr<CustomDialog>;

class CustomDialog : public CreatedGui
{
    Q_OBJECT
public:
    explicit CustomDialog(const QString & jsName, const QString & uiXml, Basemenu* pBaseMenu, const QString & confirmText = "");
    virtual ~CustomDialog();

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
    /**
     * clears all ui elements and loads the default xml
    */
    Q_INVOKABLE void refreshUi();
private slots:
    void remove();

protected:
    QString m_jsName;
    QString m_uiXml;
    Basemenu* m_pBaseMenu{nullptr};
    ScriptVariables m_Variables;
};

Q_DECLARE_INTERFACE(CustomDialog, "CustomDialog");
