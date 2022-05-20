#pragma once

#include <QObject>
#include <QTimer>

#include "3rd_party/oxygine-framework/oxygine-framework.h"

#include "ui_reader/createdgui.h"

class Basemenu;
using spBasemenu = oxygine::intrusive_ptr<Basemenu>;

class Basemenu : public CreatedGui
{
    Q_OBJECT
public:
    explicit Basemenu();
    virtual ~Basemenu() = default;
public slots:
    bool getFocused() const;
    virtual void setFocused(bool Focused);
protected slots:
    virtual void onEnter() = 0;
protected:
    bool m_Focused{true};
private:
    QTimer m_onEnterTimer;
};

Q_DECLARE_INTERFACE(Basemenu, "Basemenu");
