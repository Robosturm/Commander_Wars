#pragma once

#include <QObject>

#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"
#include "coreengine/scriptvariables.h"

class UiFactory;
class CreatedGui;
using spCreatedGui = oxygine::intrusive_ptr<CreatedGui>;

class CreatedGui : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit CreatedGui();
    virtual ~CreatedGui();
    void addFactoryUiItem(oxygine::spActor pItem);

    template<class TClass>
    TClass* getCastedObject(const QString & id)
    {
        return oxygine::safeCast<TClass*>(getObject(id));
    }
    virtual oxygine::spActor loadCustomId(const QString & item, qint32 x, qint32 y, bool enabled, bool visible, float scale,
                                          const QString & id, const QString & tooltip, const QString & onEvent,
                                          UiFactory* pFactoty, CreatedGui* pMenu, qint32 loopIdx, qint32 & scaledWidth, qint32 & scaledHeight)
    {
        return oxygine::spActor();
    }
signals:
    void sigOnUpdate();
    void sigFinished();
public slots:
    virtual void setEnabled(bool value) override;
    void resetUi();
    void loadXml(QString xmlFile);
    void setObjectEnabled(const QString id, bool value);
    void showFileDialog(const QStringList & wildcards, const QString & startFolder, bool isSaveDialog, const QString & jsObject, const QString & jsCallback, QString startFile = "", bool preview = false, QString acceptButtonName = tr("Ok"));
    void showFolderDialog(const QString & startFolder, const QString & jsObject, const QString & jsCallback);
    void showTextInputDialog(const QString & text, bool showCancel, const QString & startInput, const QString & jsObject, const QString & jsCallback);    
    QObject* getObject(const QString & id);
    void restart();
    qint32 getUiWidth() const;
    qint32 getUiHeight() const;    
    void createSubUi(const QString & uiXml, CreatedGui* pBaseUi);
    void exit();
    CreatedGui* getUiParent();
    /**
     * @brief getVariables
     * @return
     */
    inline ScriptVariables* getVariables()
    {
        return &m_Variables;
    }
private slots:
    void remove();
protected:
    QVector<oxygine::spActor> m_factoryUiItem;
    CreatedGui* m_pBaseUi{nullptr};
    ScriptVariables m_Variables;
};

Q_DECLARE_INTERFACE(CreatedGui, "CreatedGui");
