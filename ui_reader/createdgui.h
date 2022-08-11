#pragma once

#include <QObject>

#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"

class CreatedGui;
using spCreatedGui = oxygine::intrusive_ptr<CreatedGui>;

class CreatedGui : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit CreatedGui() = default;
    virtual ~CreatedGui();
    void addFactoryUiItem(oxygine::spActor pItem);
signals:
    void sigOnUpdate();
public slots:
    virtual void setEnabled(bool value) override;
    void resetUi();
    void loadXml(QString xmlFile);
    void setObjectEnabled(const QString id, bool value);
    void showFileDialog(const QStringList & wildcards, const QString & startFolder, const QString & jsObject, const QString & jsCallback, QString startFile = "", bool preview = false, QString acceptButtonName = tr("Ok"));
    void showFolderDialog(const QString & startFolder, const QString & jsObject, const QString & jsCallback);
    void showTextInputDialog(const QString & text, bool showCancel, const QString & startInput, const QString & jsObject, const QString & jsCallback);
    QObject* getObject(const QString & id);
    void restart();

protected:
    QVector<oxygine::spActor> m_factoryUiItem;
};

Q_DECLARE_INTERFACE(CreatedGui, "CreatedGui");
