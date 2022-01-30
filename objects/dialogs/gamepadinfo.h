#pragma once

#include <QObject>

#include "3rd_party/oxygine-framework/oxygine-framework.h"
#include "ui_reader/createdgui.h"

class GamepadInfo;
using spGamepadInfo = oxygine::intrusive_ptr<GamepadInfo>;

class GamepadInfo : public CreatedGui
{
    Q_OBJECT
public:
    explicit GamepadInfo();
    virtual ~GamepadInfo();
public slots:
    void remove();
};

