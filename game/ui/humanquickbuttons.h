#pragma once

#include <QObject>
#include "ui_reader/createdgui.h"

class HumanQuickButtons;
using spHumanQuickButtons = oxygine::intrusive_ptr<HumanQuickButtons>;

class HumanQuickButtons : public CreatedGui
{
public:
    explicit HumanQuickButtons();
    virtual ~HumanQuickButtons() = default;
};

