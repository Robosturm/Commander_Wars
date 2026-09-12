#pragma once

#include <QObject>
#include <QTimer>

#include "ui_reader/createdgui.h"

class Basemenu;
using spBasemenu = std::shared_ptr<Basemenu>;

class Basemenu : public CreatedGui
{
    Q_OBJECT
public:
    explicit Basemenu();
    virtual ~Basemenu() = default;
    Q_INVOKABLE bool getFocused() const;
    Q_INVOKABLE  virtual void setFocused(bool Focused);
    Q_INVOKABLE void showCustomDialog(const QString & jsName, const QString & uiXml, const QString & confirmText = tr("Ok"), bool contentString = false);
protected slots:
    virtual void onEnter() = 0;
protected:
    bool m_Focused{true};
    QTimer m_onEnterTimer;
};

Q_DECLARE_INTERFACE(Basemenu, "Basemenu");
