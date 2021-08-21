#ifndef BASEMENU_H
#define BASEMENU_H

#include <QObject>
#include <QTimer>
#include "3rd_party/oxygine-framework/oxygine-framework.h"

class Basemenu;
using spBasemenu = oxygine::intrusive_ptr<Basemenu>;

class Basemenu : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit Basemenu();
    virtual ~Basemenu();

    void addFactoryUiItem(oxygine::spActor pItem);
signals:
    void sigOnUpdate();
public slots:
    bool getFocused() const;
    virtual void setFocused(bool Focused);
protected slots:
    virtual void onEnter() = 0;
protected:
    bool m_Focused{true};

private:
    QVector<oxygine::spActor> m_factoryUiItem;
    QTimer m_onEnterTimer;
};

#endif // BASEMENU_H
