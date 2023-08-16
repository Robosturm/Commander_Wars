#pragma once

#include <QObject>

#include "ui_reader/createdgui.h"

class GamepadInfo;
using spGamepadInfo = std::shared_ptr<GamepadInfo>;

class GamepadInfo final : public CreatedGui
{
    Q_OBJECT
public:
    explicit GamepadInfo();
   virtual ~GamepadInfo();
public slots:
    void remove();
};

Q_DECLARE_INTERFACE(GamepadInfo, "GamepadInfo");
