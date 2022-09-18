#pragma once

#include <QObject>

#include "3rd_party/oxygine-framework/oxygine/core/intrusive_ptr.h"
#include "ui_reader/createdgui.h"

class GamepadInfo;
using spGamepadInfo = oxygine::intrusive_ptr<GamepadInfo>;

class GamepadInfo final : public CreatedGui
{
    Q_OBJECT
public:
    explicit GamepadInfo();
    ~GamepadInfo();
public slots:
    void remove();
};

Q_DECLARE_INTERFACE(GamepadInfo, "GamepadInfo");
