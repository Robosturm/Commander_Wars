#pragma once

#include <QObject>

#include "3rd_party/oxygine-framework/oxygine-framework.h"

class CreatedGui;
using spCreatedGui = oxygine::intrusive_ptr<CreatedGui>;

class CreatedGui : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit CreatedGui() = default;
    virtual ~CreatedGui();
    void addFactoryUiItem(oxygine::spActor pItem);
    void resetUi();
signals:
    void sigOnUpdate();
public slots:
    virtual void setEnabled(bool value) override;
protected:
    QVector<oxygine::spActor> m_factoryUiItem;
};

Q_DECLARE_INTERFACE(CreatedGui, "CreatedGui");
