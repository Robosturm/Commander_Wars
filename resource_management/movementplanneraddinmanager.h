#pragma once

#include "resource_management/ressourcemanagement.h"

class MovementPlannerAddInManager final : public QObject, public RessourceManagement<MovementPlannerAddInManager>
{
    Q_OBJECT
public:

protected:
    friend RessourceManagement<MovementPlannerAddInManager>;
    MovementPlannerAddInManager();

private:
    ~MovementPlannerAddInManager() = default;
};

