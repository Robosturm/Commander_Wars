#pragma once

#include <QObject>
#include <QTimer>

#include "ui_reader/createdgui.h"

class Basemenu;
using spBasemenu = oxygine::intrusive_ptr<Basemenu>;

class Basemenu : public CreatedGui
{
    Q_OBJECT
public:
    explicit Basemenu();
    virtual ~Basemenu() = default;
   Q_INVOKABLE bool getFocused() const;
   Q_INVOKABLE  virtual void setFocused(bool Focused);
protected slots:
    virtual void onEnter() = 0;
protected:
    bool m_Focused{true};
    QTimer m_onEnterTimer;
};

Q_DECLARE_INTERFACE(Basemenu, "Basemenu");
