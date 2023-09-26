#pragma once

#include <QObject>
#include "ui_reader/createdgui.h"

class GameMenue;
class HumanQuickButtons;
using spHumanQuickButtons = std::shared_ptr<HumanQuickButtons>;

class HumanQuickButtons final : public CreatedGui
{
public:
    explicit HumanQuickButtons(GameMenue* pMenu);
    virtual ~HumanQuickButtons();
};

