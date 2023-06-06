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
public:
    Q_INVOKABLE virtual void setEnabled(bool value) override;
    Q_INVOKABLE void resetUi();
    Q_INVOKABLE void loadXml(QString xmlFile);
    Q_INVOKABLE void setObjectEnabled(const QString id, bool value);
    Q_INVOKABLE void showFileDialog(const QStringList wildcards, const QString startFolder, bool isSaveDialog, const QString jsObject, const QString jsCallback, QString startFile = "", bool preview = false, QString acceptButtonName = tr("Ok"));
    Q_INVOKABLE void showFolderDialog(const QString startFolder, const QString jsObject, const QString jsCallback);
    Q_INVOKABLE void showTextInputDialog(const QString text, bool showCancel, const QString startInput, const QString jsObject, const QString jsCallback);
    Q_INVOKABLE QObject* getObject(const QString id);
    Q_INVOKABLE void restart();
    Q_INVOKABLE qint32 getUiWidth() const;
    Q_INVOKABLE qint32 getUiHeight() const;
    Q_INVOKABLE void createSubUi(const QString uiXml, CreatedGui* pBaseUi);
    Q_INVOKABLE void exit();
    Q_INVOKABLE CreatedGui* getUiParent();
    /**
     * @brief getVariables
     * @return
     */
    Q_INVOKABLE inline ScriptVariables* getVariables()
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
